#include "TimeUp.h"

#include <system/EventListener.h>
#include <UILayoutComponent.h>
#include <UILayout.h>
#include <Actor.h>

using namespace Tako;

void TimeUp::BeginPlay()
{
    subscriptions_ = EventListener::GetInstance()->Subscribe<Event::TimeUp>([this](const Event::TimeUp& event)
    {
        OnTimeUp(event);
    });
}

void TimeUp::OnTimeUp(const Event::TimeUp& event)
{
    if (auto component = GetOwner()->GetComponent<UILayoutComponent>(); component)
    {
        component->GetLayout().SetVisible("Text", true);
    }
}
