#include "Eater.h"

#include <Actor.h>

#include "Plate.h"
#include "Script/HUD/SatisfactionGauge/SatisfactionGauge.h"

#ifdef _DEBUG
#include "imgui.h"

bool Eater::DrawCustomImGui()
{
    // 保存する値（TPROPERTY）を変えたら true を返す
    bool changed = false;

    const ImVec4 kGood = { 0.35f, 0.85f, 0.45f, 1.f };
    const ImVec4 kBad  = { 0.95f, 0.35f, 0.35f, 1.f };
    const ImVec4 kDim  = { 0.60f, 0.60f, 0.60f, 1.f };

    // ---- 状態 ----
    ImGui::SeparatorText("Status");
    ImGui::TextColored(active_ ? kGood : kDim, active_ ? "EATING" : "STOPPED");
    ImGui::SameLine();
    if (ImGui::SmallButton(active_ ? "Stop" : "Start"))
    {
        active_ = !active_;
        changed = true;
    }

    // つなぎ先が取れているか（Tick と同じ取り方で確かめる）
    Tako::Actor* plateActor = GetOwner()->Resolve(plate_);
    Tako::Actor* gaugeActor = GetOwner()->Resolve(gauge_);
    const bool   hasPlate   = plateActor && plateActor->GetScript<Plate>();
    const bool   hasGauge   = gaugeActor && gaugeActor->GetScript<SatisfactionGauge>();
    ImGui::TextColored(hasPlate ? kGood : kBad, hasPlate ? "Plate : OK" : "Plate : missing");
    ImGui::SameLine(0.f, 24.f);
    ImGui::TextColored(hasGauge ? kGood : kBad, hasGauge ? "Gauge : OK" : "Gauge : missing");
    ImGui::Text("Pending : %.2f", pends_);

    return changed;
}
#endif

void Eater::Tick([[maybe_unused]] float deltaTime)
{
    if (!active_) return;

    elapsed_ += deltaTime;

    while (interval_ <= elapsed_)
    {
        elapsed_ -= interval_;
        pends_ += amount_;
    }

    Eat();
}

void Eater::Eat()
{
    if(pends_ <= 0.f) return;

    auto plate = GetOwner()->Resolve(plate_)->GetScript<Plate>();
    auto gauge = GetOwner()->Resolve(gauge_)->GetScript<SatisfactionGauge>();

    if(!plate || !gauge) return;

    const float taken = plate->Take(pends_);
    gauge->Charge(taken * percentMul);

    pends_ = 0.f;
}
