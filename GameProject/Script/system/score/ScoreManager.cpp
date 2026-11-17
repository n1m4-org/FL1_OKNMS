#include "ScoreManager.h"

#include <system/score/ScoreView.h>
#include <Actor.h>

#include <utility/imgui/ImWidgetWrapper.h>

#ifdef _DEBUG
bool ScoreManager::DrawCustomImGui()
{
    static int amount = 0;
    ImGui::InputInt("##scoreInput", &amount);
    ImGui::SameLine();
    bool add = ImGui::ButtonColored("Add", Tako::ColorHSV(0.33f, 0.7f, 0.7f, 1.0f));
    ImGui::SameLine();
    bool sub = ImGui::ButtonColored("Subtract", Tako::ColorHSV(0.66f, 0.7f, 0.7f, 1.0f));

    if (add)
    {
        AddScore(amount);
    }
    if (sub)
    {
        SubtractScore(amount);
    }

    return add || sub;
}
#endif // _DEBUG

void ScoreManager::AddScore(uint32_t value)
{
    if (value == 0) return;

    score_ += value;
    ApplyToView();
}

void ScoreManager::SubtractScore(uint32_t value)
{
    if (value == 0) return;

    score_ -= std::min(value, score_.Get());
    ApplyToView();
}

void ScoreManager::ApplyToView()
{
    auto pActor = GetOwner()->Resolve(viewActor_);
    if (!pActor) return;

    auto pScript = pActor->GetScript<ScoreView>();
    if (!pScript) return;

    pScript->ApplyScore(score_);
}
