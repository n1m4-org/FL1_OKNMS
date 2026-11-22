#pragma once
#include <Script.h>
#include <event/TimeLimitEvent.h>
#include <system/EventSubscription.h>
#include <optional>


TCLASS()
class TimeUp : public Tako::Script
{
    GENERATED_BODY()

public:
    void BeginPlay() override;

private:
    void OnTimeUp(const Event::TimeUp& event);

    std::optional<EventSubscription> subscriptions_;
};
