#include "debug_imgui_camera.h"

#include <iomanip>

#include "lib/imgui.h"

#include <sstream>
#include <vector>
#include <fstream>

#include "debug_ostream.h"

static std::string CAMERA_PRESET_FILE = "camera_preset.csv";

// camera data
static DirectX::XMFLOAT3 g_CameraFront;
static DirectX::XMFLOAT3 g_CameraUp;
static DirectX::XMFLOAT3 g_CameraRight;
static DirectX::XMFLOAT3 g_CameraPosition;
static float g_CameraFov;

static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraPositionChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraFrontChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraUpChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraRightChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 front, DirectX::XMFLOAT3 up)> g_OnCameraPresetApply = nullptr;
static std::function<void(float fov)> g_OnFovChanged = nullptr;


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

void DebugImGui_Camera_Initialize()
{
    std::ifstream file(CAMERA_PRESET_FILE);

    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // skip header

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string item;
            CameraPreset preset;

            // label
            std::getline(ss, preset.label, ',');

            auto split = [ss_ = &ss](DirectX::XMFLOAT3& float3) -> void
            {
                char comma;
                float x, y, z;
                (*ss_) >> x >> comma;
                (*ss_) >> y >> comma;
                (*ss_) >> z >> comma;
                float3 = { x, y, z };
            };

            split(preset.front);
            split(preset.up);
            split(preset.right);
            split(preset.position);
            g_CameraPresets.push_back(preset);
        }
        g_CameraPresetIndex = g_CameraPresets.size() - 1;
    }
}

void DebugImGui_Camera_Finalize() {}

void DebugImGui_UpdateCameraData(
    DirectX::XMFLOAT3 camera_front,
    DirectX::XMFLOAT3 camera_up,
    DirectX::XMFLOAT3 camera_right,
    DirectX::XMFLOAT3 camera_position,
    float camera_fov
    )
{
    g_CameraFront = camera_front;
    g_CameraUp = camera_up;
    g_CameraRight = camera_right;
    g_CameraPosition = camera_position;
    g_CameraFov = camera_fov;
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

void DebugImGui_SetOnCameraPresetApply(const std::function<void(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 front, DirectX::XMFLOAT3 up)>& callback)
{
    g_OnCameraPresetApply = callback;
}

void DebugImGui_SetOnCameraFovChanged(const std::function<void(float fov)>& callback)
{
    g_OnFovChanged = callback;
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

    void DebugImGui_CameraSavePresetsToFile()
    {
        std::ofstream file(CAMERA_PRESET_FILE);
        if (file.is_open())
        {
            file << "Label,front_x,front_y,front_z,up_x,up_y,up_z,right_x,right_y,right_z,position_x,position_y,position_z\n";

            for (auto p : g_CameraPresets)
            {
                file << std::setprecision(8);
                file << p.label << "," << p.front.x << "," << p.front.y << "," << p.front.z << "," << p.up.x << "," << p.up.y << "," << p.up.z << "," << p.right.x << "," << p.right.y << "," << p.right.z << "," << p.position.x << "," << p.position.y << "," << p.position.z << "\n";
            }

            file.close();
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
                DebugImGui_CameraSavePresetsToFile();
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

                            if (g_OnCameraPresetApply)
                            {
                                g_OnCameraPresetApply(g_CameraPosition, g_CameraFront, g_CameraUp);
                            }
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

    void DebugImGui_CameraFovWidget()
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("FOV");
        ImGui::SameLine();
        bool edited = ImGui::DragFloat(("##camera fov"), &g_CameraFov, 0.5f, 1.0f, 180.0f);

        if (edited)
        {
            g_OnFovChanged(g_CameraFov);
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
    DebugImGui_CameraFovWidget();
    DebugImGui_CameraPresetsWidget();
}
