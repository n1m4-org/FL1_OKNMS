#include "EventDispatcher.h"

#include "Source/system/EventListener.h"

#ifdef TAKO_EDITOR
#include <imgui.h>
#include <utility/imgui/ImWidgetWrapper.h>
#endif // TAKO_EDITOR

void EventDispatcher::BeginPlay()
{
    EventListener::GetInstance()->SetOnDispatchedCallback([this](const std::string& eventName)
    {
        dispatchedEventNames_.push_back(eventName);
    });
}

void EventDispatcher::LateTick([[maybe_unused]] float deltaTime)
{
    // イベントのディスパッチ
    // キューに溜まったイベントは、このタイミングで処理されます
    EventListener::GetInstance()->Dispatch();
}

#ifdef TAKO_EDITOR
bool EventDispatcher::DrawCustomImGui()
{
    ImGui::Text("Event Dispached Log");

    ImGui::BeginChild("Event Dispached Log", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& eventName : dispatchedEventNames_)
    {
        ImGui::Text("%s", eventName.c_str());
    }

    ImGui::EndChild();

    return false;
}
#endif // TAKO_EDITOR
