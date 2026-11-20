#pragma once

#include <Script.h>
#include <type/Combo.h>
#include <ActorRef.h>

TCLASS()
class ComboManager : public Tako::Script
{
    GENERATED_BODY()

public:
    void AddCombo(uint32_t val = 1u) { combo_ += val; }
    void ResetCombo() { combo_ = 0; }

private:
    void ApplyToView();

    Combo combo_ = 0;

    TPROPERTY(Edit, Required, ActorWith = "ComboView", Tooltip = "スコア表示用のActor")
    Tako::ActorRef viewActor_;
};
