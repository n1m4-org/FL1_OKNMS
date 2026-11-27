#pragma once
#include <array>
#include <optional>

#include "Script.h"

#include "ActorRef.h"
#include "Plate.h"

/// <summary>
/// かみさまが食べる動作
/// 指定したテンポで食べる。
/// </summary>
TCLASS()
class Eater : public Tako::Script {
    GENERATED_BODY()

    TPROPERTY(Edit, Required, ActorWith = "SatisfactionGauge")
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

    Plate* pPlate_ = nullptr;

    float elapsed_ = 0.f;

    float pends_ = 0.f;

    std::optional<float> slideTarget_;
    float slideFrom_ = 0.f;
    float slideTime_ = 1.f;
    float slideElapsed_ = 0.f;

#ifdef TAKO_EDITOR
    // デバッグメニュー用（保存しない）
    float debugTargetBpm_ = 120.f;
    float debugSlideTime_ = 1.f;
    // BPM の移り変わりのグラフ（古い物から上書きする）
    std::array<float, 180> debugBpmHistory_{};
    size_t                 debugBpmHistoryHead_ = 0;
#endif

public: //メンバー関数

#ifdef TAKO_EDITOR
    bool DrawCustomImGui() override;
#endif

    void Tick(float deltaTime) override;

    void SetPlate(Plate* plate);

    void SetTempo(float sec, float time = 1.f);
    void SetTempoFromBpm(float bpm, float time = 1.f);

    /** @brief 食べる状態変更
     * @param active 食べるかbool
     */
    void Activate(bool active);

    bool IsSatisfied() const;

    void SetGaugeVisible(bool visible) const;

private:
    void Eat();

    void SlideTempo(float deltaTime);
};
