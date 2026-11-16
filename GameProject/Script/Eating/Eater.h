#pragma once
#include <array>
#include <optional>

#include "Script.h"

#include "ActorRef.h"

/// <summary>
/// 
/// </summary>
TCLASS()
class Eater : public Tako::Script {
    GENERATED_BODY()

    TPROPERTY(Edit, Required, ActorWith = "Plate")
    Tako::ActorRef plate_;

    TPROPERTY(Edit, Required, ActorWith = "Gauge")
    Tako::ActorRef gauge_;

    TPROPERTY(Edit)
    bool active_ = false;

    TPROPERTY(Edit, ClampMin = 0, Speed = 0.1)
    float amount_ = 1.f;

    // ClampMin 0.05 秒 = BPM 1200 まで
    TPROPERTY(Edit, ClampMin = 0.05, Speed = 0.01, Tooltip = "1 口の間隔（秒）。60 / BPM")
    float interval_ = 1.f;

    TPROPERTY(Edit, Speed = 0.1)
    float percentMul = 1.f;


    float elapsed_ = 0.f;

    float pends_ = 0.f;

    std::optional<float> slideTarget_;
    float slideFrom_ = 0.f;
    float slideTime_ = 1.f;
    float slideElapsed_ = 0.f;

#ifdef _DEBUG
    // デバッグメニュー用（保存しない）
    float debugTargetBpm_ = 120.f;
    float debugSlideTime_ = 1.f;
    // BPM の移り変わりのグラフ（古い物から上書きする）
    std::array<float, 180> debugBpmHistory_{};
    size_t                 debugBpmHistoryHead_ = 0;
#endif

public: //メンバー関数

#ifdef _DEBUG
    bool DrawCustomImGui() override;
#endif

    void Tick(float deltaTime) override;

    void SetTempo(float sec, float time = 1.f);
    void SetTempoFromBpm(float bpm, float time = 1.f);

private:
    void Eat();

    void SlideTempo(float deltaTime);
};
