#include "debug_imgui_camera.h"

#include <iomanip>

#include "lib/imgui.h"

#include <sstream>
#include <vector>

#include "debug_ostream.h"

// camera data
static DirectX::XMFLOAT3 g_CameraFront;
static DirectX::XMFLOAT3 g_CameraUp;
static DirectX::XMFLOAT3 g_CameraRight;
static DirectX::XMFLOAT3 g_CameraPosition;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraPositionChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraFrontChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraUpChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraRightChanged = nullptr;

struct CameraPreset
{
    std::string label;
    DirectX::XMFLOAT3 front;
    DirectX::XMFLOAT3 up;
    DirectX::XMFLOAT3 right;
    DirectX::XMFLOAT3 position;
};

static std::vector<CameraPreset> g_CameraPresets = {};
static int g_CameraPresetIndex = -1;

void DebugImGui_UpdateCameraData(
    DirectX::XMFLOAT3 camera_front,
    DirectX::XMFLOAT3 camera_up,
    DirectX::XMFLOAT3 camera_right,
    DirectX::XMFLOAT3 camera_position
    )
{
    g_CameraFront = camera_front;
    g_CameraUp = camera_up;
    g_CameraRight = camera_right;
    g_CameraPosition = camera_position;
}

void DebugImGui_SetOnCameraPositionChanged(const std::function<void(DirectX::XMFLOAT3 camera_position)>& callback)
{
    g_OnCameraPositionChanged = callback;
}

void DebugImGui_SetOnCameraFrontChanged(const std::function<void(DirectX::XMFLOAT3 camera_front)>& callback)
{
    g_OnCameraFrontChanged = callback;
}

void DebugImGui_SetOnCameraUpChanged(const std::function<void(DirectX::XMFLOAT3 camera_up)>& callback)
{
    g_OnCameraUpChanged = callback;
}

void DebugImGui_SetOnCameraRightChanged(const std::function<void(DirectX::XMFLOAT3 camera_right)>& callback)
{
    g_OnCameraRightChanged = callback;
}

namespace
{
    void DebugImGui_CameraPositionWidget(const std::string& label, const std::string& id, float (&float3)[3], std::function<void(DirectX::XMFLOAT3 cb)> cb)
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text(label.c_str());
        ImGui::SameLine();

        bool edited = ImGui::DragFloat3(("##" + id).c_str(), float3, 0.01f);
        if (edited && cb)
        {
            DirectX::XMFLOAT3 float3_ = { float3[0], float3[1], float3[2] };
            cb(float3_);
        }
        if (ImGui::BeginPopupContextItem(("##" + id + "_pop").c_str()))
        {
            ImGui::SeparatorText(label.c_str());
            ImGui::Separator();
            if (ImGui::MenuItem("Reset to 0, 0, 0"))
            {
                float3[0] = 0;
                float3[1] = 0;
                float3[2] = 0;

                if (cb)
                {
                    const DirectX::XMFLOAT3 float3_ = { float3[0], float3[1], float3[2] };
                    cb(float3_);
                }
            }
            if (ImGui::MenuItem("Copy current value"))
            {
                std::stringstream ss;
                ss << std::setprecision(3);
                ss << float3[0] << ", " << float3[1] << ", " << float3[2];
                ImGui::SetClipboardText(ss.str().c_str());
            }
            ImGui::EndPopup();
        }
    }

    void DebugImGui_CameraOrientationWidget(const std::string& label, const std::string& id, float (&float3)[3], std::function<void(DirectX::XMFLOAT3 cb)> cb)
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text(label.c_str());
        ImGui::SameLine();

        bool edited = ImGui::DragFloat3(("##" + id).c_str(), float3, 0.01f);
        if (edited && cb)
        {
            DirectX::XMFLOAT3 float3_ = { float3[0], float3[1], float3[2] };
            cb(float3_);
        }
        if (ImGui::BeginPopupContextItem(("##" + id + "_pop").c_str()))
        {
            ImGui::SeparatorText(label.c_str());
            ImGui::Separator();
            if (ImGui::MenuItem("Copy current value"))
            {
                std::stringstream ss;
                ss << std::setprecision(3);
                ss << float3[0] << ", " << float3[1] << ", " << float3[2];
                ImGui::SetClipboardText(ss.str().c_str());
            }
            ImGui::EndPopup();
        }
    }

    void DebugImGui_CameraPresetsWidget()
    {
        if (ImGui::TreeNode("Camera Preset"))
        {
            static char label_buf[256] = "";

            if (ImGui::Button("Save"))
            {
                for (auto p : g_CameraPresets)
                {
                    if (p.label == std::string(label_buf))
                    {
                        label_buf[0] = '\0'; // same label cause same id make error
                    }
                }

                g_CameraPresets.push_back({
                    label_buf[0] == '\0' ? std::to_string(++g_CameraPresetIndex) : std::string(label_buf),
                    g_CameraFront,
                    g_CameraUp,
                    g_CameraRight,
                    g_CameraPosition
                });
                label_buf[0] = '\0';
            }

            ImGui::SameLine();
            ImGui::InputTextWithHint("##presets_name",
                                     ("Presets name, default: " + std::to_string(g_CameraPresetIndex + 1)).c_str(),
                                     label_buf, IM_ARRAYSIZE(label_buf));

            if (g_CameraPresets.size() > 0)
            {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Load");

                int item_to_delete = -1;
                for (int i = 0; i < g_CameraPresets.size(); i++)
                {
                    ImGui::SameLine();
                    if (ImGui::Button(g_CameraPresets[i].label.c_str()))
                    {
                        if (g_OnCameraFrontChanged && g_OnCameraRightChanged && g_OnCameraUpChanged && g_OnCameraPositionChanged)
                        {
                            g_CameraPosition = g_CameraPresets[i].position;
                            g_CameraFront = g_CameraPresets[i].front;
                            g_CameraUp = g_CameraPresets[i].up;
                            g_CameraRight = g_CameraPresets[i].right;

                            g_OnCameraPositionChanged(g_CameraPosition);
                            g_OnCameraFrontChanged(g_CameraFront);
                            g_OnCameraUpChanged(g_CameraUp);
                            g_OnCameraRightChanged(g_CameraRight);
                        }
                    }
                    if (ImGui::BeginPopupContextItem(("##button_ctx_menu" + g_CameraPresets[i].label + "_delete_pop").c_str()))
                    {
                        if (ImGui::MenuItem(("Delete presets " + g_CameraPresets[i].label).c_str()))
                        {
                            item_to_delete = i;
                        }
                        ImGui::EndPopup();
                    }
                }

                if (item_to_delete >= 0 && item_to_delete < g_CameraPresets.size())
                {
                    g_CameraPresets.erase(g_CameraPresets.begin() + item_to_delete);
                }
            }

            ImGui::TreePop();
            ImGui::Spacing();
        }
    }
}

void DebugImGui_CameraUpdate()
{
    ImGui::SeparatorText("Camera Attribute");

    float camera_position[3] = { g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z };
    float camera_front[3] = { g_CameraFront.x, g_CameraFront.y, g_CameraFront.z };
    float camera_up[3] = { g_CameraUp.x, g_CameraUp.y, g_CameraUp.z };
    float camera_right[3] = { g_CameraRight.x, g_CameraRight.y, g_CameraRight.z };

    DebugImGui_CameraPositionWidget("Camera Position", "cam_pos", camera_position, g_OnCameraPositionChanged);
    DebugImGui_CameraOrientationWidget("Camera Front   ", "cam_front", camera_front, g_OnCameraFrontChanged);
    DebugImGui_CameraOrientationWidget("Camera Up      ", "cam_up", camera_up, g_OnCameraUpChanged);
    DebugImGui_CameraOrientationWidget("Camera Right   ", "cam_right", camera_right, g_OnCameraRightChanged);
    DebugImGui_CameraPresetsWidget();
}
