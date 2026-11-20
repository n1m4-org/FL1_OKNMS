#pragma once

#ifdef TAKO_EDITOR
#include <imgui.h>

#include <ColorFunc.h>

namespace ImGui
{
    bool ButtonColored(const char* label, Tako::ColorHSV col, const ImVec2& size = ImVec2(0, 0));
    void TextRed(const char* fmt, ...);
    void TextGreen(const char* fmt, ...);
    void TextBlue(const char* fmt, ...);
    void TextYellow(const char* fmt, ...);
}

#endif // TAKO_EDITOR
