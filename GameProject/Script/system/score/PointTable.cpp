#include "PointTable.h"



#ifdef _DEBUG
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
#endif // _DEBUG
