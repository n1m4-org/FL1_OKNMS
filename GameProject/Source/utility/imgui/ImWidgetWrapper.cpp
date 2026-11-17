#include "ImWidgetWrapper.h"
#include <cstdarg>
#include <ColorFunc.h>

#ifdef _DEBUG

bool ImGui::ButtonColored(const char* label, Tako::ColorHSV col, const ImVec2& size)
{
    Tako::Color rgba = {};
    Tako::ColorHSV hsv = {};
    
    /// ボタン背景色
    rgba = col.ToColor();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(rgba.r, rgba.g, rgba.b, rgba.a));

    /// ホバー色 
    hsv.h = col.h; hsv.s = col.s + 0.1f; hsv.v = col.v + 0.1f;
    rgba = hsv.ToColor();
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(rgba.r, rgba.g, rgba.b, rgba.a));

    /// アクティブ色
    hsv.s += 0.1f; hsv.v += 0.1f;
    rgba = hsv.ToColor();
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(rgba.r, rgba.g, rgba.b, rgba.a));

    /// テキスト色
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    // ボタン本体
    bool flag = ImGui::Button(label, size);

    // 後始末
    ImGui::PopStyleColor(4);

    return flag;
}

void ImGui::TextRed(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    static constexpr auto color = Tako::Color::FromHex(0xe0453b);
    ImGui::TextColoredV(ImVec4(color.r, color.g, color.b, color.a), fmt, args);
    va_end(args);
}

void ImGui::TextGreen(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    static constexpr auto color = Tako::Color::FromHex(0x57e03b);
    ImGui::TextColoredV(ImVec4(color.r, color.g, color.b, color.a), fmt, args);
    va_end(args);
}

void ImGui::TextBlue(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    static constexpr auto color = Tako::Color::FromHex(0x3b92e0);
    ImGui::TextColoredV(ImVec4(color.r, color.g, color.b, color.a), fmt, args);
    va_end(args);
}

void ImGui::TextYellow(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    static constexpr auto color = Tako::Color::FromHex(0xe0ce3b);
    ImGui::TextColoredV(ImVec4(color.r, color.g, color.b, color.a), fmt, args);
    va_end(args);
}

#endif //_DEBUG
