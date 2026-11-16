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

    TPROPERTY(Edit, ClampMin = 1, Speed = 0.1)
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

private:
    void Eat();
};
