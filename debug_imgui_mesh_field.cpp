#include "debug_imgui_mesh_field.h"

#include <sstream>
#include "lib/imgui.h"

MeshFieldData current;

MeshFieldDataCallbackFn g_callback_fn;

void DebugImGui_MeshField_Initialize() {}

void DebugImGui_MeshField_Finalize() {}

void DebugImGui_UpdateMeshFieldData(const MeshFieldData& data)
{
    current = data;
}

void DebugImgui_MeshField_SetCallback(const MeshFieldDataCallbackFn& callback_fn)
{
    g_callback_fn = callback_fn;
}

void DebugImGui_MeshFieldUpdate()
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Is display grid ");
    ImGui::SameLine();
    if(ImGui::Checkbox("##grid_is_display", &current.is_display))
    {
        if (g_callback_fn.is_display_fn)
            g_callback_fn.is_display_fn(current.is_display);
    }
    
    std::stringstream ss;
    ss << "Mesh Field: " << current.g_x_count << " x " << current.g_z_count << "(" << current.g_x_count * current.g_size_x << " x " << current.g_size_z * current.g_z_count << ")";
    ImGui::Text(ss.str().c_str());
    
    int grid_count[2] = { current.g_x_count, current.g_z_count };
    float grid_size[2] = { current.g_size_x, current.g_size_z };
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Grid Count");
    ImGui::SameLine();
    if (ImGui::DragInt2("##grid_count", grid_count, 1, 0))
    {
        if (g_callback_fn.count_x_fn)
            g_callback_fn.count_x_fn(grid_count[0]);
        if (g_callback_fn.count_z_fn)
            g_callback_fn.count_z_fn(grid_count[1]);
        
        current.g_x_count = grid_count[0];
        current.g_z_count = grid_count[1];
    }
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Grid Size ");
    ImGui::SameLine();
    if (ImGui::DragFloat2("##grid_size", grid_size, 0.01f))
    {
        if (g_callback_fn.size_x_fn)
            g_callback_fn.size_x_fn(grid_size[0]);
        if (g_callback_fn.size_z_fn)
            g_callback_fn.size_z_fn(grid_size[1]);
        
        current.g_size_x = grid_size[0];
        current.g_size_z = grid_size[1];
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Grid Y    ");
    ImGui::SameLine();
    if (ImGui::DragFloat("##grid_y", &current.g_pos_y, 0.01f))
    {
        if (g_callback_fn.y_pos_fn)
            g_callback_fn.y_pos_fn(current.g_pos_y);
    }
}
