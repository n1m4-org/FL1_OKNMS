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

    GameLog::Info(*this, "Saving high scores to \"{}\"", kPathString);

    /// アクタの解決
    auto actor = GetOwner()->Resolve(tableActor_);
    if (!actor)
    {
        GameLog::Error(*this, "Could not resolve the table actor. (Check \"tableActor\" in the inspector)");
        return;
    }

    /// スクリプト取得
    auto table = actor->GetScript<HighScoreTable>();
    if (!table)
    {
        GameLog::Error(*this, "Actor \"{}\" has no HighScoreTable script", actor->GetName());
        return;
    }

    /// ディレクトリを作成する
    std::error_code ec;
    std::filesystem::create_directories(tableDirectoryPath_, ec);
    if (ec)
    {
        GameLog::Error(*this, "Failed to create directory \"{}\": {}", kPath.parent_path().generic_string(), ec.message());
        return;
    }

    /// ファイルストリームを開く
    std::ofstream ofs(tableDirectoryPath_ + tableFileName_);
    if (!ofs.is_open())
    {
        GameLog::Error(*this, "Failed to open \"{}\" for writing ", kPathString);
        return;
    }

    /// JSONに変換して書き込む
    nlohmann::json j = table->ToJSON();
    ofs << j.dump(4);

    if (ofs.fail())
    {
        GameLog::Error(*this, "Failed to write high scores to \"{}\"", kPathString);
        ofs.close();
        return;
    }
    ofs.close();

    GameLog::Info(*this, "Saved {} high scores to \"{}\"", table->size(), kPathString);
}

void HighScoreStorage::LoadHighScoreTable()
{
    const fs::path kPath = fs::path(tableDirectoryPath_) / tableFileName_;
    const std::string kPathString = kPath.generic_string();

    /// アクタの解決
    auto actor = GetOwner()->Resolve(tableActor_);
    if (!actor)
    {
        GameLog::Error(*this, "Could not resolve the table actor. (Check \"tableActor\" in the inspector)");
        return;
    }

    /// スクリプト取得
    auto table = actor->GetScript<HighScoreTable>();
    if (!table)
    {
        GameLog::Error(*this, "Actor \"{}\" has no HighScoreTable script", actor->GetName());
        return;
    }

    /// ファイルストリームを開く
    std::ifstream ifs(tableDirectoryPath_ + tableFileName_);
    if (!ifs.is_open())
    {
        GameLog::Error(*this, "Failed to open \"{}\" for reading ", kPathString);
        return;
    }

    /// ストリームからJSONを読み込む
    nlohmann::json j;
    ifs >> j;
    if (ifs.fail())
    {
        GameLog::Error(*this, "Failed to load high scores to \"{}\"", kPathString);
        ifs.close();
        return;
    }
    ifs.close();

    /// JSONからハイスコアテーブルを復元する
    table->FromJSON(j);

    GameLog::Info(*this, "Loaded {} high scores from \"{}\"", table->size(), kPathString);
}
