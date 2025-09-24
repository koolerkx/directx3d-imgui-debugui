#include "debug_imgui_render.h"

#include "direct3d.h"
#include "lib/imgui.h"

namespace 
{
    bool is_wireframe = false;
}

void DebugImGui_Render_Initialize() {}
void DebugImGui_Render_Finalize() {}

void DebugImGui_RenderUpdate()
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Wireframe");
    ImGui::SameLine();
    if(ImGui::Checkbox("##grid_is_display", &is_wireframe))
    {
        Direct3D_RSWireframe(is_wireframe);
    }
}
