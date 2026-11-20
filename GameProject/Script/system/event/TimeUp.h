#pragma once
#include <Script.h>
#include <event/TimeLimitEvent.h>
#include <system/EventSubscription.h>


TCLASS()
class TimeUp : public Tako::Script
{
    GENERATED_BODY()

public:
    void BeginPlay() override;

private:
    void OnTimeUp(const Event::TimeUp& event);

    EventSubscription subscriptions_;
};
