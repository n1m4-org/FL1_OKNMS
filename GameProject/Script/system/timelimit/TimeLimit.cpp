#include "TimeLimit.h"

#include <Actor.h>
#include <UILayout.h>
#include <UILayoutComponent.h>
#include <event/TimeLimitEvent.h>
#include <string>

#ifdef TAKO_EDITOR
#include <imgui.h>
#include <utility/imgui/ImWidgetWrapper.h>
#endif // TAKO_EDITOR

using namespace Tako;

void TimeLimit::Tick(float deltaTime)
{
    switch (state_)
    {
    case State::Running:
        // 残り時間を減らす
        DecreaseTime(deltaTime);

        // 残り時間が 0 になったらカウントを止める
        CountStopIfFinished();

        // 残り時間が 0 になったらイベントを発行する
        PublishEventIfFinished();

        break;

    case State::Ready:
    case State::Paused:
        break;
    }
}

#ifdef TAKO_EDITOR
bool TimeLimit::DrawCustomImGui()
{
    bool start = ImGui::ButtonColored("Start", Tako::ColorHSV(0.66f, 0.7f, 0.7f, 1.0f), ImVec2(60, 25));
    ImGui::SameLine();
    bool pause = ImGui::ButtonColored("Pause", Tako::ColorHSV(0.16f, 0.7f, 0.7f, 1.0f), ImVec2(60, 25));
    ImGui::SameLine();
    bool reset = ImGui::ButtonColored("Reset", Tako::ColorHSV(0.00f, 0.7f, 0.7f, 1.0f), ImVec2(60, 25));

    if (start) Start();
    else if (pause) Pause();
    else if (reset) Reset();

    return false;
}
#endif // TAKO_EDITOR

void TimeLimit::Start()
{
    state_ = State::Running;
}

void TimeLimit::Pause()
{
    state_ = State::Paused;
}

void TimeLimit::Reset()
{
    state_ = State::Ready;
    remainingTime_ = maxLimitTime_;
}

void TimeLimit::AddRemainingTime(float time)
{
    remainingTime_ += time;
}

void TimeLimit::BeginPlay()
{
    remainingTime_ = maxLimitTime_;
}

void TimeLimit::DecreaseTime(float time)
{
    remainingTime_ -= time;
}

void TimeLimit::CountStopIfFinished()
{
    if (remainingTime_ <= 0.0f)
    {
        remainingTime_ = 0.0f;
        state_ = State::Ready;
    }
}

void TimeLimit::PublishEventIfFinished() const
{
    if (remainingTime_ <= 0.0f)
    {
        EventListener::GetInstance()->Publish<Event::TimeUp>({});
    }
}
