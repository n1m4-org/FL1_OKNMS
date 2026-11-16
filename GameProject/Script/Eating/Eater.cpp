#include "Eater.h"

#include <Actor.h>

#include "Plate.h"
#include "Script/HUD/SatisfactionGauge/SatisfactionGauge.h"

#ifdef _DEBUG
#include "imgui.h"

#include <algorithm>
#include <cstdio>
#include <iterator>

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

    // ---- テンポ ----
    ImGui::SeparatorText("Tempo");

    constexpr float kMinBpm = 1.f;
    constexpr float kMaxBpm = 1200.f;   // interval_ の ClampMin 0.05 秒
    float bpm = interval_ > 0.f ? 60.f / interval_ : kMinBpm;
    if (ImGui::DragFloat("BPM", &bpm, 0.5f, kMinBpm, kMaxBpm, "%.1f"))
    {
        SetTempo(60.f / std::clamp(bpm, kMinBpm, kMaxBpm));
        changed = true;
    }
    ImGui::TextDisabled("= %.3f sec / bite", interval_);

    // よく使う BPM と、半分・倍
    constexpr float kPresets[] = { 60.f, 90.f, 120.f, 150.f, 180.f };
    for (size_t i = 0; i < std::size(kPresets); ++i)
    {
        if (i > 0) ImGui::SameLine();
        char label[16];
        std::snprintf(label, sizeof(label), "%.0f", kPresets[i]);
        if (ImGui::Button(label))
        {
            SetTempo(60.f / kPresets[i]);
            changed = true;
        }
    }
    if (ImGui::Button("x0.5"))
    {
        SetTempo(60.f / std::clamp(bpm * 0.5f, kMinBpm, kMaxBpm));
        changed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("x2"))
    {
        SetTempo(60.f / std::clamp(bpm * 2.f, kMinBpm, kMaxBpm));
        changed = true;
    }

    // 拍：ランプは拍の直後に明るく、次の拍へ向けて暗くなる。バーは次の拍までの進み
    const float progress = interval_ > 0.f ? std::clamp(elapsed_ / interval_, 0.f, 1.f) : 0.f;
    const float lamp     = active_ ? 1.f - progress : 0.f;
    const float radius   = ImGui::GetFrameHeight() * 0.5f;
    const ImVec2 pos     = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddCircleFilled(
        { pos.x + radius, pos.y + radius }, radius * 0.8f,
        ImGui::GetColorU32(ImVec4(1.f, 0.6f, 0.2f, 0.15f + 0.85f * lamp)));
    ImGui::Dummy({ radius * 2.f, radius * 2.f });
    ImGui::SameLine();
    ImGui::ProgressBar(progress, { -1.f, radius * 2.f }, "");

    // ---- 1 口 ----
    ImGui::SeparatorText("Bite");
    const float gaugePerBite = amount_ * percentMul;
    ImGui::Text("Plate  -%.2f / bite", amount_);
    ImGui::Text("Gauge  +%.2f%% / bite", gaugePerBite);
    ImGui::Text("Gauge  +%.2f%% / sec", interval_ > 0.f ? gaugePerBite / interval_ : 0.f);

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

void Eater::SetTempo(float sec)
{
    interval_ = sec;
}

void Eater::SetTempoFromBpm(float bpm)
{
    SetTempo(60.f / bpm);
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
