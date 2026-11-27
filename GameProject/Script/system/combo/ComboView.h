#pragma once

#include <Script.h>
#include <type/Combo.h>

TCLASS()
class ComboView : public Tako::Script
{
    GENERATED_BODY()

public:
    void Tick(float deltaTime) override;

    void Apply(Combo combo);

private:
    void UpdateText();

    uint32_t combo_ = 0;
    uint32_t comboDisplayed_ = 0;
};
