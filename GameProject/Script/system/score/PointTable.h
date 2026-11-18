#pragma once

#include <Script.h>
#include <type/Score.h>
#include <system/score/PointAction.h>
#include <vector>

TSTRUCT()
struct PointEntry
{
    GENERATED_STRUCT_BODY()

    TPROPERTY(Edit)
    PointAction action = PointAction::SatisfiedGod;

    TPROPERTY(Edit)
    Score score;
};

TCLASS()
class PointTable : public Tako::Script
{
    GENERATED_BODY()

public:
    bool DrawCustomImGui() override;

private:
    TPROPERTY(Edit, Tooltip = "ポイントテーブルのエントリ")
    std::vector<PointEntry> points_;
};