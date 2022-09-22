#include "custom.hpp"

#include "imgui_tricks.hpp"

using namespace ImGui;

bool Custom1337::tab(const char* label, bool selected) {
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiID id = window->GetID(label);
    ImVec2 labelSize = CalcTextSize(label);
    ImVec2 p = window->DC.CursorPos;
    ImRect totalBb(p, { p.x + labelSize.x, p.y + labelSize.y });

    ItemSize(totalBb);
    ItemAdd(totalBb, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(totalBb, id, &hovered, &held);

    float textAnim = ImTricks::Animations::FastFloatLerp(std::string(label).append("tab.textAnim"), selected, 0.25f, 1.f, GetIO().DeltaTime * 4);
    float alphaAnim = ImTricks::Animations::FastFloatLerp(std::string(label).append("tab.alphaAnim"), selected, 0.f, 1.f, GetIO().DeltaTime * 4);

    window->DrawList->AddRectFilled({ totalBb.Min.x + 10, window->Pos.y + 61 }, { totalBb.Max.x - 10, window->Pos.y + 65 }, ImColor(123 / 255.f, 123 / 255.f, 233 / 255.f, alphaAnim), 10, ImDrawFlags_RoundCornersTop);

    PushStyleColor(ImGuiCol_Text, GetColorU32(ImGuiCol_Text, textAnim));
    RenderText(totalBb.Min, label);
    PopStyleColor();

    return pressed;
}

void Custom1337::beginChild(const char* name, ImVec2 size) {
    ImGuiWindow* window = GetCurrentWindow();

    ImVec2 labelSize = CalcTextSize(name);
    ImVec2 p = window->DC.CursorPos;

    BeginChild(std::string(name).append("main").c_str(), size, false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

    window->DrawList->AddRectFilled(p, { p.x + size.x, p.y + 45 }, ImColor(41, 41, 49), GImGui->Style.ChildRounding);
    RenderText({ p.x + size.x / 2 - labelSize.x / 2, p.y + 45 / 2 - labelSize.y / 2 }, name);

    SetCursorPos({ 0, GImGui->Style.WindowPadding.y + 45 });
    BeginChild(name, { size.x, size.y - 45 - GImGui->Style.WindowPadding.y }, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    SetCursorPosX(GImGui->Style.WindowPadding.x);
    BeginGroup();
}

void Custom1337::endChild() {
    EndGroup();
    EndChild();
    EndChild();
}

void Custom1337::renderFrame(ImVec2 start, ImVec2 end, ImColor frameColor, ImColor borderColor, float rounding) {
    PushStyleColor(ImGuiCol_Border, borderColor.Value);
    RenderFrame(start, end, frameColor, true, rounding);
    PopStyleColor();
}
