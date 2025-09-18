#include "debug_imgui_camera.h"

#include <iomanip>

#include "lib/imgui.h"

#include <sstream>

// camera data
static DirectX::XMFLOAT3 g_CameraFront;
static DirectX::XMFLOAT3 g_CameraUp;
static DirectX::XMFLOAT3 g_CameraRight;
static DirectX::XMFLOAT3 g_CameraPosition;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraPositionChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraFrontChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraUpChanged = nullptr;
static std::function<void(DirectX::XMFLOAT3 cb)> g_OnCameraRightChanged = nullptr;


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
}
