#include "imgui.h"
#include "imgui_internal.h"

namespace Custom1337 {
    bool tab(const char* label, bool selected);

    void beginChild(const char* name, ImVec2 size);
    void endChild();

    void renderFrame(ImVec2 start, ImVec2 end, ImColor frameColor, ImColor borderColor, float rounding);
};
