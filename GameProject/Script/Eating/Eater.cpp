#include "Eater.h"

#include <Actor.h>
#include <EaseFunc.h>

#include "Plate.h"
#include "Script/HUD/SatisfactionGauge/SatisfactionGauge.h"
#include "AudioSourceComponent.h"

#ifdef TAKO_EDITOR
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
    Tako::Actor* gaugeActor = GetOwner()->Resolve(gauge_);
    const bool   hasGauge   = gaugeActor && gaugeActor->GetScript<SatisfactionGauge>();
    ImGui::TextColored(hasGauge ? kGood : kBad, hasGauge ? "Gauge : OK" : "Gauge : missing");
    ImGui::Text("Pending : %.2f", pends_);

    // ---- テンポ ----
    ImGui::SeparatorText("Tempo");

    constexpr float kMinBpm = 1.f;
    constexpr float kMaxBpm = 1200.f;   // interval_ の ClampMin 0.05 秒
    const float bpm = interval_ > 0.f ? 60.f / interval_ : kMinBpm;

    // 今の BPM（スライド中は毎フレーム変わる）
    ImGui::Text("Now  %.1f BPM", bpm);
    ImGui::SameLine();
    ImGui::TextDisabled("(%.3f sec / bite)", interval_);

    // 目標の BPM とスライドの時間を決めて、Slide で始める
    ImGui::DragFloat("Target BPM", &debugTargetBpm_, 0.5f, kMinBpm, kMaxBpm, "%.1f");
    ImGui::DragFloat("Slide Time", &debugSlideTime_, 0.05f, 0.f, 30.f, "%.2f sec");
    if (ImGui::Button("Slide"))
    {
        SetTempoFromBpm(std::clamp(debugTargetBpm_, kMinBpm, kMaxBpm), debugSlideTime_);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(runs in Play while Active)");

    // よく使う BPM と、今の半分・倍。押すと Slide Time でスライドする
    const auto slideTo = [&](float target)
    {
        debugTargetBpm_ = std::clamp(target, kMinBpm, kMaxBpm);
        SetTempoFromBpm(debugTargetBpm_, debugSlideTime_);
    };
    constexpr float kPresets[] = { 60.f, 90.f, 120.f, 150.f, 180.f };
    for (size_t i = 0; i < std::size(kPresets); ++i)
    {
        if (i > 0) ImGui::SameLine();
        char label[16];
        std::snprintf(label, sizeof(label), "%.0f", kPresets[i]);
        if (ImGui::Button(label)) slideTo(kPresets[i]);
    }
    if (ImGui::Button("x0.5")) slideTo(bpm * 0.5f);
    ImGui::SameLine();
    if (ImGui::Button("x2")) slideTo(bpm * 2.f);

    // スライドの状態：始め → 目標 と、進み具合
    if (slideTarget_ && slideFrom_ > 0.f && *slideTarget_ > 0.f)
    {
        const float slideProgress = slideTime_ > 0.f ? std::clamp(slideElapsed_ / slideTime_, 0.f, 1.f) : 1.f;
        char overlay[64];
        std::snprintf(overlay, sizeof(overlay), "%.1f -> %.1f  (%.2f / %.2f sec)",
                      60.f / slideFrom_, 60.f / *slideTarget_, slideElapsed_, slideTime_);
        ImGui::TextColored(kGood, "SLIDING");
        ImGui::SameLine();
        ImGui::ProgressBar(slideProgress, { -1.f, 0.f }, overlay);
    }
    else
    {
        ImGui::TextColored(kDim, "STEADY");
    }

    // BPM の移り変わり。イージングのカーブがそのまま見える
    if (debugBpmHistory_[debugBpmHistoryHead_] <= 0.f)
    {
        // 最初の表示では、今の BPM で埋めておく（0 からの線が出ないように）
        debugBpmHistory_.fill(bpm);
    }
    debugBpmHistory_[debugBpmHistoryHead_] = bpm;
    debugBpmHistoryHead_ = (debugBpmHistoryHead_ + 1) % debugBpmHistory_.size();
    const auto [minIt, maxIt] = std::minmax_element(debugBpmHistory_.begin(), debugBpmHistory_.end());
    const float graphMin = *minIt - 5.f;
    const float graphMax = *maxIt + 5.f;
    char graphLabel[32];
    std::snprintf(graphLabel, sizeof(graphLabel), "%.0f - %.0f", *minIt, *maxIt);
    ImGui::PlotLines("##bpmHistory", debugBpmHistory_.data(), static_cast<int>(debugBpmHistory_.size()),
                     static_cast<int>(debugBpmHistoryHead_), graphLabel, graphMin, graphMax,
                     { -1.f, ImGui::GetFrameHeight() * 3.f });

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

void Eater::Tick(float deltaTime)
{
    if (!active_) return;

    SlideTempo(deltaTime);

    elapsed_ += deltaTime;

    while (interval_ <= elapsed_)
    {
        elapsed_ -= interval_;
        pends_ += amount_;
    }

    Eat();
}

void Eater::SetPlate(Plate* plate) {
    pPlate_ = plate;
}

void Eater::SetTempo(float sec, float time) 
{
    slideFrom_ = interval_;
    slideElapsed_ = 0.f;
    slideTarget_ = sec;
    slideTime_ = time;
}

void Eater::SetTempoFromBpm(float bpm, float time)
{
    SetTempo(60.f / bpm, time);
}

void Eater::Activate(bool active) 
{
    active_ = active;
}

bool Eater::IsSatisfied() const 
{
    auto actor = GetOwner()->Resolve(gauge_);
    auto* gauge = actor ? actor->GetScript<SatisfactionGauge>() : nullptr;
    return gauge && gauge->IsFull();
}

void Eater::SetGaugeVisible(bool visible) const
{
    if (Tako::Actor* gauge = GetOwner()->Resolve(gauge_))
    {
        gauge->SetActive(visible);
    }
}

void Eater::Eat() 
{
    if(pends_ <= 0.f) return;

    auto actor = GetOwner()->Resolve(gauge_);
    auto* gauge = actor ? actor->GetScript<SatisfactionGauge>() : nullptr;

    if(!pPlate_ || !gauge) return;

    const float taken = pPlate_->Take(pends_);
    gauge->Charge(taken * percentMul);

    pends_ = 0.f;

    if(auto audio = GetOwner()->GetComponent<Tako::AudioSourceComponent>())
    {
        audio->Play();
    }
}

void Eater::SlideTempo(float deltaTime) 
{
    if (!slideTarget_)return;

    slideElapsed_ += deltaTime;

    const float t = std::clamp(slideElapsed_ / slideTime_, 0.f, 1.f);

    const float fromBpm = 60.f / slideFrom_;
    const float destBpm = 60.f / *slideTarget_;
    const float bpm = Tako::Vec3::Lerp(fromBpm, destBpm, Tako::Ease::InOutSine(t));

    interval_ = 60.f / bpm;

    if (1.f <= t)
    {
        interval_ = *slideTarget_;
        slideTarget_.reset();
    }
}
