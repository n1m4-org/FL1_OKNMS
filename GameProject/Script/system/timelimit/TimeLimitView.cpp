#include "TimeLimitView.h"

#include <UILayoutComponent.h>
#include <Actor.h>
#include <UILayout.h>
#include <utility/GameLog.h>


using namespace Tako;

void TimeLimitView::BeginPlay()
{
    /// TimeLimitスクリプトを取得する
    if (pTimeLimit_ = GetTimeLimitScript(); !pTimeLimit_)
    {
        GameLog::Error(*this, "TimeLimit スクリプトが見つかりませんでした。");
    }

    /// UILayoutComponentが設定されているか確認する
    if (!GetOwner()->GetComponent<UILayoutComponent>())
    {
        GameLog::Error(*this, "UILayoutComponent が設定されていません。");
    }
}

void TimeLimitView::Tick(float deltaTime)
{
    // 残り時間を UI に反映する
    UpdateText();
}

TimeLimit* TimeLimitView::GetTimeLimitScript() const
{
    Actor* timeLimitActor = GetOwner()->Resolve(timeLimitActor_);

    if (!timeLimitActor)
    {
        GameLog::Error(*this, "TimeLimitActor の解決に失敗しました。ActorRefが設定されているか確認してください。");
        return nullptr;
    }

    return timeLimitActor->GetScript<TimeLimit>();
}

void TimeLimitView::UpdateText()
{
    Actor* owner = GetOwner();
    assert(owner != nullptr);

    UILayoutComponent* ulc = owner->GetComponent<UILayoutComponent>();

    if (!pTimeLimit_ || !ulc) return;

    const float kRemainingTime = pTimeLimit_->GetRemainingTime();

    /// 残り時間を分と秒に変換して表示する
    float remainingMin = kRemainingTime / 60.0f;
    remainingMin = std::floor(remainingMin);
    float remainingSec = kRemainingTime - static_cast<int>(remainingMin) * 60.0f;
    remainingSec = std::floor(remainingSec);

    /// UI に残り時間を反映する (秒は 0埋め 2桁表示にする)
    ulc->GetLayout().SetText("Time", std::format("{}:{:02}", remainingMin, remainingSec));
}
