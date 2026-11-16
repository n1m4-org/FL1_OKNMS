#pragma once
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

public: //メンバー関数

#ifdef _DEBUG
    bool DrawCustomImGui() override;
#endif

    void Tick(float deltaTime) override;

    void SetTempo(float sec);
    void SetTempoFromBpm(float bpm);

private:
    void Eat();
};
