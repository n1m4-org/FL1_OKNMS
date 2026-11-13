#include "EventDispatcher.h"

#include "Source/system/EventListener.h"

void EventDispatcher::LateTick([[maybe_unused]] float deltaTime)
{
    // イベントのディスパッチ
    // キューに溜まったイベントは、このタイミングで処理されます
    EventListener::GetInstance()->Dispatch();
}
