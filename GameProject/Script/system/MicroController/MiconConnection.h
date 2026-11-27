#pragma once
#include <Script.h>

#include <Windows.h>

#include <string>
#include <vector>

/// <summary>
/// MiconConnectionは、マイクロコントローラーとシリアル通信でやり取りするクラスです。
/// COM ポートを順に試して自分のマイコンを見つけ、つながった後はデータを読み、マイコンへ送ることもできます。
/// </summary>
TCLASS(Order = 0)
class MiconConnection : public Tako::Script
{
    GENERATED_BODY()

public:

    ~MiconConnection();

    void Awake() override;

    void Tick(float deltaTime) override;

    void EndPlay() override;

private:

    bool SendLine(const char* msg) const;
    void HandleLine(const std::string& str);
    // 開いていれば閉じる
    void Close();

    bool Open(const std::string& portName);
    bool ReadLines();

    std::vector<std::string> GetSerialPortList() const;

private:

    enum class State
    {
        Searching,
        Probing,
        Connected
    } state_ = State::Searching;

    HANDLE handle_ = INVALID_HANDLE_VALUE;  // 初期化しないと、Awake の前に消えたとき不定の値を CloseHandle する

    std::string line_;  // 改行が来るまで貯めるバッファ（フレームをまたいで 1 行が届くことがあるのでメンバーにする）

    
    float probeElapsed_ = 0.0f;   // 返事を待っている時間(probingに入ってからの時間)
    float whoElapsed_   = 0.0f;   // 最後に WHO を送ってからの時間

    // 取得したポート一覧
    std::vector<std::string> candidates_;
    // 使用するポート名
    std::string portName_;
        
};
