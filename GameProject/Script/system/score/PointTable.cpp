#include "PointTable.h"



#ifdef TAKO_EDITOR
#include <imgui.h>
#include <utility/imgui/ImWidgetWrapper.h>
bool PointTable::DrawCustomImGui()
{
    bool add = ImGui::ButtonColored("Add Entry", Tako::ColorHSV(0.33f, 0.5f, 0.7f, 1.0f));

    if (add)
    {
        PointEntry newEntry{};
        points_.push_back(std::move(newEntry));
    }

    return add;
}

std::optional<Score> PointTable::Get(PointAction action)
{
    auto it = std::ranges::find(points_, action, &PointEntry::action);
    if (it != points_.end())
    {
        return it->score;
    }
    
    return std::nullopt;
}

#endif // TAKO_EDITOR
