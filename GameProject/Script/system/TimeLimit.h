#pragma once

#include <Script.h>

TCLASS()
class TimeLimit : public Tako::Script
{
    GENERATED_BODY()

public:
    enum class State { Ready, Stopped, Running };

    void Tick(float deltaTime) override;

private:
    TPROPERTY(Edit, ClampMin = 100000);
    float maxLimitTime_ = 60.0f;

    TPROPERTY(Visible);
    float remainingTime_ = 0.0f;

    TPROPERTY(Visible);
    State status_ = State::Ready;
};