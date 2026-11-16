#include "TimeLimit.h"

#include <Actor.h>
#include <UILayout.h>
#include <UILayoutComponent.h>
#include <string>

#ifdef _DEBUG
#include <imgui.h>
#include <utility/imgui/ImWidgetWrapper.h>
#endif // _DEBUG

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
        break;

    case State::Ready:
    case State::Paused:
        break;
    }

    // 残り時間を UI に反映する
    if (isDirty_) UpdateText();
}

#ifdef _DEBUG
bool TimeLimit::DrawCustomImGui()
{
    ImGui::Text("Dirty : "); ImGui::SameLine();
    if (isDirty_) ImGui::TextRed("true");
    else ImGui::TextGreen("false");

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
#endif // _DEBUG

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
    isDirty_ = true;
}

void TimeLimit::AddRemainingTime(float time)
{
    remainingTime_ += time;
    isDirty_ = true;
}

void TimeLimit::DecreaseTime(float time)
{
    remainingTime_ -= time;
    isDirty_ = true;
}

void TimeLimit::CountStopIfFinished()
{
    if (remainingTime_ <= 0.0f)
    {
        remainingTime_ = 0.0f;
        isDirty_ = true;
        state_ = State::Ready;
    }
}

void TimeLimit::UpdateText()
{
    UILayoutComponent* ulc = GetOwner()->GetComponent<UILayoutComponent>();
    ulc->GetLayout().SetText("Time", std::to_string(static_cast<int>(remainingTime_)));
    isDirty_ = false;
}
