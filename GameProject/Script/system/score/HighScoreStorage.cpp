#include "HighScoreStorage.h"
#include <Actor.h>
#include <system/score/HighScoreTable.h>
#include <system/EventListener.h>
#include <event/TimeLimitEvent.h>
#include <utility/GameLog.h>
#include <fstream>

#include <filesystem>

namespace fs = std::filesystem;

void HighScoreStorage::BeginPlay()
{
    LoadHighScoreTable();

    subscription_ = EventListener::GetInstance()->Subscribe<Event::TimeUp>([this](const Event::TimeUp& event)
    {
        SaveHighScoreTable();
    });
}

void HighScoreStorage::SaveHighScoreTable() const
{
    const fs::path kPath = fs::path(tableDirectoryPath_) / tableFileName_;
    const std::string kPathString = kPath.generic_string();

    GameLog::Info(*this, "ハイスコアの保存を開始します。 \"{}\"", kPathString);

    /// アクタの解決
    auto actor = GetOwner()->Resolve(tableActor_);
    if (!actor)
    {
        GameLog::Error(*this, "HighScoreTableアクターの解決に失敗しました。(インスペクターで \"tableActor\" が設定されているか確認してください。)");
        return;
    }

    /// スクリプト取得
    auto table = actor->GetScript<HighScoreTable>();
    if (!table)
    {
        GameLog::Error(*this, "\"{}\"アクター has no HighScoreTable script", actor->GetName());
        return;
    }

    /// ディレクトリを作成する
    std::error_code ec;
    std::filesystem::create_directories(tableDirectoryPath_, ec);
    if (ec)
    {
        GameLog::Error(*this, "ディレクトリの作成に失敗しました。 \"{}\": {}", kPath.parent_path().generic_string(), ec.message());
        return;
    }

    /// ファイルストリームを開く
    std::ofstream ofs(tableDirectoryPath_ + tableFileName_);
    if (!ofs.is_open())
    {
        GameLog::Error(*this, "書き込みに失敗しました。 ファイルを開けません。 \"{}\"", kPathString);
        return;
    }

    /// JSONに変換して書き込む
    nlohmann::json j = table->ToJSON();
    ofs << j.dump(4);

    if (ofs.fail())
    {
        GameLog::Error(*this, "ハイスコアの書き込みに失敗しました。 \"{}\"", kPathString);
        ofs.close();
        return;
    }
    ofs.close();

    GameLog::Info(*this, "{} の high scores を保存しました。保存場所 : \"{}\"", table->size(), kPathString);
}

void HighScoreStorage::LoadHighScoreTable()
{
    const fs::path kPath = fs::path(tableDirectoryPath_) / tableFileName_;
    const std::string kPathString = kPath.generic_string();

    /// アクタの解決
    auto actor = GetOwner()->Resolve(tableActor_);
    if (!actor)
    {
        GameLog::Error(*this, "HighScoreTableアクターの解決に失敗しました。(インスペクターで \"tableActor\" が設定されているか確認してください。)");
        return;
    }

    /// スクリプト取得
    auto table = actor->GetScript<HighScoreTable>();
    if (!table)
    {
        GameLog::Error(*this, "\"{}\"アクター has no HighScoreTable script", actor->GetName());
        return;
    }

    /// ファイルストリームを開く
    std::ifstream ifs(tableDirectoryPath_ + tableFileName_);
    if (!ifs.is_open())
    {
        GameLog::Error(*this, "読み込みに失敗しました。 ファイルを開けません。 \"{}\"", kPathString);
        return;
    }

    /// ストリームからJSONを読み込む
    nlohmann::json j;
    ifs >> j;
    if (ifs.fail())
    {
        GameLog::Error(*this, "ハイスコアの読み込みに失敗しました。 \"{}\"", kPathString);
        ifs.close();
        return;
    }
    ifs.close();

    /// JSONからハイスコアテーブルを復元する
    table->FromJSON(j);

    GameLog::Info(*this, "{} の high scores を復元しました。保存場所 : \"{}\"", table->size(), kPathString);
}
