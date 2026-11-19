#pragma once

#include <Script.h>
#include <type/Score.h>
#include <system/score/PointAction.h>
#include <vector>
#include <optional>

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
#ifdef _DEBUG
    bool DrawCustomImGui() override;
#endif // _DEBUG

    /// <summary>
    /// 指定されたアクションに対応するスコアを取得します。
    /// </summary>
    /// <param name="action">スコアを取得する対象のアクション。</param>
    /// <returns>対応するスコア。スコアが存在しない場合は空のstd::optionalを返します。</returns>
    std::optional<Score> Get(PointAction action);

private:
    TPROPERTY(Edit, Tooltip = "ポイントテーブルのエントリ")
    std::vector<PointEntry> points_;
};