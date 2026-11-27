#include "MiconConnection.h"

#include <Windows.h>
#include <Logger.h>

#include <Source/system/Microcontroller/MiconParse.h>
#include <Source/system/EventListener.h>

namespace
{
    // COMポートへ問いかける文字列
    constexpr const char* kQueryString = "WHO";

    // COMポートからの期待する文字列(これが返ってきたらものを使用する)
    constexpr const char* kMiconId = "OKNMS";

    constexpr float kProbeTimeout = 3.0f;   // これだけ待っても返事が無ければ、次のポートへ
    constexpr float kWhoInterval  = 0.5f;   // WHO を送り直す間隔
}

MiconConnection::~MiconConnection()
{
    Close();  // EndPlay が呼ばれずに消えたとき用
}

void MiconConnection::Awake()
{
    Close();       // 前の Play で開いた物が残っていれば閉じる
    line_.clear();

    probeElapsed_ = 0.0f;

    state_ = State::Searching;
    candidates_.clear();

}

void MiconConnection::EndPlay()
{
    Close();
}

bool MiconConnection::SendLine(const char* msg) const
{
    if (handle_ == INVALID_HANDLE_VALUE || msg == nullptr)
    {
        return false;
    }

    DWORD writenSize = 0;
    // 文字列に改行を追加する
    std::string sendStr = std::string(msg) + '\n';
    if (!WriteFile(handle_, sendStr.c_str(), DWORD(sendStr.size()), &writenSize, nullptr))
    {
        return false;
    }
    if (writenSize != sendStr.size())
    {
        return false;
    }

    return true;
}

void MiconConnection::HandleLine(const std::string& str)
{
    if (str == kMiconId)
    {
        if (state_ != State::Connected)
        {
            // 不要なログを出さないように、Connected になったときだけログを出す
            Tako::Logger::Log("Micon Connected");
            state_ = State::Connected;
        }
        return;
    }

    if (state_ != State::Connected)
        return;

    if (auto result = MiconParser::Parse(str); result)
    {
        EventListener::GetInstance()->Publish(*result);
        // デバッグ用ログ
        Tako::Logger::Log("Micon Event : %s", str.c_str());
    }
    else
    {
        // 解釈できない文字列 ログに出す
        Tako::Logger::Log("Unknown line : [%s]", str.c_str());
    }
}

void MiconConnection::Close()
{
    if (handle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
    }
}

bool MiconConnection::Open(const std::string& portName)
{
    std::string path = "\\\\.\\" + portName;
    handle_ = CreateFileA(path.c_str(),      // 開くもののパス
                          GENERIC_READ | GENERIC_WRITE,       // 読み書き
                          0,                  // 共有なし
                          nullptr,            // セキュリティの設定
                          OPEN_EXISTING,      // 既存のものを開く
                          0,                  //
                          nullptr             // テンプレートなし
    );

    if (handle_ == INVALID_HANDLE_VALUE)
    {
        // handleの取得に失敗した
        Tako::Logger::Log("%lu", GetLastError());
        Tako::Logger::Log("handleの取得に失敗");
        return false;
    }

    // ComStateの取得
    DCB dcb{};
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(handle_, &dcb))
    {
        Tako::Logger::Log("%lu", GetLastError());
        Tako::Logger::Log("ComStateの取得に失敗");
        Close();
        return false;
    }
    dcb.BaudRate    = CBR_115200;           // 1秒間に115200ビットの通信速度
    dcb.ByteSize    = 8;                    // 1文字を8ビットで表す
    dcb.Parity      = NOPARITY;             // パリティなし
    dcb.StopBits    = ONESTOPBIT;           // 1文字の終わりに1ビットのストップビットを付与
    dcb.fDtrControl = DTR_CONTROL_ENABLE;   // DTR制御を有効にする
    // DTR信号:PC側の準備完了信号
    if (!SetCommState(handle_, &dcb))
    {
        Tako::Logger::Log("%lu", GetLastError());
        Tako::Logger::Log("ComStateの設定に失敗");
        Close();
        return false;
    }

    // 読むときの設定
    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout        = MAXDWORD; // 文字と文字の間の最大待機時間(ms
    timeouts.ReadTotalTimeoutMultiplier = 0;        // 可変待ち時間(バイト当たりms
    timeouts.ReadTotalTimeoutConstant   = 0;        // 固定待ち時間(ms
    timeouts.WriteTotalTimeoutConstant  = 20;       // 書き込みの固定待ち時間(ms
    // この設定では来ているバイトだけ読み取り即座に戻る

    SetCommTimeouts(handle_, &timeouts);

    PurgeComm(handle_, PURGE_RXCLEAR);  // 開く前に溜まっていた古いデータを捨てる

    line_.clear();
    portName_ = portName;
    Tako::Logger::Log("Open %s", portName_.c_str());
    return true;
}

bool MiconConnection::ReadLines()
{
    if (handle_ == INVALID_HANDLE_VALUE)
        return false;

    // 今届いている分だけ読む。届いている物が無くなったら（bytesRead == 0）抜ける
    for (;;)
    {
        char buf[256];
        DWORD bytesRead = 0;
        if (!ReadFile(handle_, buf, sizeof(buf), &bytesRead, nullptr))
        {
            Tako::Logger::Log("read failed: error %lu", GetLastError());  // USB が抜けた等
            return false;
        }
        if (bytesRead == 0)
        {
            break;  // このフレームで読める物は全部読んだ
        }
        for (DWORD i = 0; i < bytesRead; ++i)
        {
            char c = buf[i];
            if (c == '\r')
                continue;

            if (c == '\n')
            {
                HandleLine(line_);
                line_.clear();
            }
            else
            {
                line_ += c;
            }
        }
    }
    return true;
}

std::vector<std::string> MiconConnection::GetSerialPortList() const
{
    std::vector<std::string> portList;

    HKEY hKey = nullptr;
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS)
    {
        return std::vector<std::string>();
    }
    for (DWORD index = 0; ; ++index)
    {
        char  deviceName[256];
        DWORD deviceNameSize = sizeof(deviceName);
        char  portName[64];
        DWORD portNameSize = sizeof(portName);
        DWORD type = 0;

        result = RegEnumValueA(hKey, index,
                               deviceName, &deviceNameSize,
                               nullptr,
                               &type,
                               reinterpret_cast<BYTE*>(portName), &portNameSize);

        if (result == ERROR_NO_MORE_ITEMS) break;           // 全部見た
        if (result != ERROR_SUCCESS || type != REG_SZ) continue;  // 取れなかった・文字列でない → 飛ばす

        if (std::string(deviceName).find("Bth") != std::string::npos) continue;   // Bluetooth は飛ばす


        portName[sizeof(portName) - 1] ='\0';
        portList.push_back(portName);

        // deviceName = "\Device\USBSER000"、portName = "COM11"
        Tako::Logger::Log("%s = %s", deviceName, portName);
    }

    RegCloseKey(hKey);

    return portList;
}

void MiconConnection::Tick(float deltaTime)
{
    switch (state_)
    {
        case MiconConnection::State::Searching:
        {
            if (candidates_.empty())
            {
                candidates_ = GetSerialPortList();
            }
            if (candidates_.empty())
            {
                return;
            }
            std::string s = candidates_.back();
            candidates_.pop_back();
            if (Open(s))
            {
                state_ = State::Probing;
                probeElapsed_ = 0.0f;
                whoElapsed_ = 0.0f;
            }
            break;
        }
        case MiconConnection::State::Probing:
        {
            if (!ReadLines())
            {
                Close();
                state_ = State::Searching;
                break;
            }
            // つながっていたら用はないので早期break
            if (state_ == State::Connected)
            {
                break;
            }

            whoElapsed_ += deltaTime;
            probeElapsed_ += deltaTime;

            // 返事の有無に関係なく、一定時間経ったら次のポートへ
            if (probeElapsed_ >= kProbeTimeout)
            {
                Close();
                state_ = State::Searching;
                break;
            }

            if (whoElapsed_ >= kWhoInterval)
            {
                if (SendLine(kQueryString))
                {
                    whoElapsed_ = 0.0f;
                }
            }

            break;
        }
        case MiconConnection::State::Connected:
        {
            if (!ReadLines())
            {
                Close();
                state_ = State::Searching;
                Tako::Logger::Log("Micon Disconnected");
            }

            DWORD   errors = 0;
            COMSTAT status{};
            if (!ClearCommError(handle_, &errors, &status))
            {
                const DWORD error = GetLastError();   // すぐ取る
                // 抜けた → ログ、Close、Searching へ

                Close();
                state_ = State::Searching;
                Tako::Logger::Log("Micon Disconnected");
                Tako::Logger::Log("ClearCommError failed: error %lu", error);
            }
            break;
        }
    }
}
