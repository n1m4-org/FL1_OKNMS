#include "ScoreView.h"
#include <algorithm>
#include <EaseFunc.h>

#include <Actor.h>
#include <UILayoutComponent.h>
#include <UILayout.h>

#ifdef _DEBUG
#include <imgui.h>
#include <utility/imgui/ImWidgetWrapper.h>
#endif // _DEBUG

using namespace Tako;

void ScoreView::Tick(float deltaTime)
{
    UpdateDisplayScore(deltaTime);

    // 前のフレームの表示スコアと保持しているスコアが違う場合はテキストを更新する
    if (scoreDisplayed_ != scoreDisplayedPrevFrame_) UpdateText();

    // 最初にTickが呼ばれてから経過した時間を更新
    now += deltaTime;
}

#ifdef _DEBUG
bool ScoreView::DrawCustomImGui()
{
    static int addScoreAmount = 0;
    ImGui::InputInt("##scoreInput", &addScoreAmount); 
    ImGui::SameLine();
    bool add = ImGui::ButtonColored("Add", Tako::ColorHSV(0.66f, 0.7f, 0.7f, 1.0f));

    if (add)
    {
        Apply(scoreTarget_ += addScoreAmount);
    }

    return true;
}
#endif // _DEBUG

void ScoreView::Apply(Score score)
{
    updatedTiming_ = now;
    scoreDisplayedLast_ = scoreDisplayed_;
    scoreTarget_ = score;
}

void ScoreView::UpdateDisplayScore(float deltaTime)
{
    float elapsedMs = now - updatedTiming_;

    float t = std::min(elapsedMs / smoothDurationMs_, 1.0f);
    float easedT = Tako::Ease::OutQuart(t);

    scoreDisplayed_ = static_cast<uint32_t>(
        std::lerp(
            static_cast<float>(scoreDisplayedLast_), 
            static_cast<float>(scoreTarget_), 
            easedT
        )
    );
}

void ScoreView::UpdateText()
{
    UILayoutComponent* ulc = GetOwner()->GetComponent<UILayoutComponent>();
    ulc->GetLayout().SetText("Score", std::to_string(static_cast<int>(scoreDisplayed_)));
    scoreDisplayedPrevFrame_ = scoreDisplayed_;
}
