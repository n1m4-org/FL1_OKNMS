#pragma once

#include <Script.h>
#include <ActorRef.h>
#include <string>
#include <system/EventSubscription.h>
#include <optional>

TCLASS()
class HighScoreStorage : public Tako::Script
{
    GENERATED_BODY()

public:
    void BeginPlay() override;

private:
    void SaveHighScoreTable() const;
    void LoadHighScoreTable();

    TPROPERTY(Edit, Tooltip = "ハイスコアを保存するディレクトリパス")
    std::string tableDirectoryPath_ = "Saves/";

    TPROPERTY(Edit, Tooltip = "ハイスコアを保存するファイルの名前")
    std::string tableFileName_ = "HighScoreTable.json";

    TPROPERTY(Edit, Required, ActorWith = "HighScoreTable", Tooltip = "テーブルのアクタ")
    Tako::ActorRef tableActor_;

    std::optional<EventSubscription> subscription_;
};
