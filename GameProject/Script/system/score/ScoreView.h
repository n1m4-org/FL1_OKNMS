#pragma once

#include <Script.h>
#include <type/Score.h>

TCLASS()
class ScoreView : public Tako::Script
{
    GENERATED_BODY()

public:
    void Tick(float deltaTime) override;


#ifdef TAKO_EDITOR
    bool DrawCustomImGui() override;
#endif // TAKO_EDITOR

    /// <summary>
    /// スコアを適用します。
    /// </summary>
    /// <param name="score">適用するスコア。</param>
    void ApplyScore(Score score);

private:
    void UpdateDisplayScore(float deltaTime);
    void UpdateText();

    TPROPERTY(Edit, ClampMin = 0.001f, Tooltip = "スコアの更新演出の時間的な長さ (ミリ秒)")
    float smoothDurationMs_ = 0.3f;

    uint32_t scoreDisplayed_ = 0;
    uint32_t scoreTarget_ = 0;

    // 前回のスコア更新時の表示スコア
    uint32_t scoreDisplayedLast_ = 0;

    // 前フレームの表示スコア
    uint32_t scoreDisplayedPrevFrame_ = 0;

    // 最初にTickが呼ばれてから経過した時間
    TPROPERTY(Visible, Transient) float now = 0.0f;

    // スコアが更新されたタイミング
    TPROPERTY(Visible, Transient) float updatedTiming_ = {};
};
