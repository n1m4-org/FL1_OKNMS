#pragma once

#include <Script.h>
#include <ActorRef.h>

#include <system/timelimit/TimeLimit.h>

TCLASS()
class TimeLimitView : public Tako::Script
{
    GENERATED_BODY()

public:
    void BeginPlay() override;
    void Tick(float deltaTime) override;

private:
    TimeLimit* GetTimeLimitScript() const;
    void UpdateText();

    TimeLimit* pTimeLimit_ = nullptr;

    TPROPERTY(Edit, Required, ActorWith = "TimeLimit", Tooltip = "TimeLimit コンポーネントを持つアクター")
    Tako::ActorRef timeLimitActor_;
};
