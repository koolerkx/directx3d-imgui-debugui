#ifndef DEBUG_IMGUI_MESH_FIELD_H
#define DEBUG_IMGUI_MESH_FIELD_H

#include <functional>

struct MeshFieldData
{
    bool is_display = true;
    float g_size_x = 0.0f;
    float g_size_z = 0.0f;
    int g_x_count = 0;
    int g_z_count = 0;
    float g_pos_y = 0.0f;
};

struct MeshFieldDataCallbackFn
{
    std::function<void(bool is_display)> is_display_fn = nullptr;
    std::function<void(float size_x)> size_x_fn = nullptr;
    std::function<void(float size_z)> size_z_fn = nullptr;
    std::function<void(int count_x)> count_x_fn = nullptr;
    std::function<void(int count_z)> count_z_fn = nullptr;
    std::function<void(float count_y)> y_pos_fn = nullptr;
};

void DebugImGui_MeshField_Initialize();
void DebugImGui_MeshField_Finalize();

void DebugImGui_UpdateMeshFieldData(const MeshFieldData& data);

void DebugImgui_MeshField_SetCallback(const MeshFieldDataCallbackFn& callback_fn);

void DebugImGui_MeshFieldUpdate();

#endif // DEBUG_IMGUI_MESH_FIELD_H
