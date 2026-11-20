#include "TimeIsUp.h"

#include <system/EventListener.h>
#include <UILayoutComponent.h>
#include <UILayout.h>
#include <Actor.h>

using namespace Tako;

void TimeIsUp::BeginPlay()
{
    subscriptions_ = EventListener::GetInstance()->Subscribe<Event::TimeUp>([this](const Event::TimeUp& event)
    {
        OnTimeUp(event);
    });
}

void TimeIsUp::OnTimeUp(const Event::TimeUp& event)
{
    if (auto component = GetOwner()->GetComponent<UILayoutComponent>(); component)
    {
        component->GetLayout().SetVisible("Text", true);
    }
}
