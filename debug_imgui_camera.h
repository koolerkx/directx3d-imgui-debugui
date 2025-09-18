#ifndef DEBUG_IMGUI_CAMERA_H
#define DEBUG_IMGUI_CAMERA_H

#include <DirectXMath.h>
#include <functional>

void DebugImGui_Camera_Initialize();
void DebugImGui_Camera_Finalize();

void DebugImGui_UpdateCameraData(
    DirectX::XMFLOAT3 camera_front,
    DirectX::XMFLOAT3 camera_up,
    DirectX::XMFLOAT3 camera_right,
    DirectX::XMFLOAT3 camera_position
    );

void DebugImGui_SetOnCameraPositionChanged(const std::function<void(DirectX::XMFLOAT3 camera_position)>& callback = {});
void DebugImGui_SetOnCameraFrontChanged(const std::function<void(DirectX::XMFLOAT3 camera_front)>& callback = {});
void DebugImGui_SetOnCameraUpChanged(const std::function<void(DirectX::XMFLOAT3 camera_up)>& callback = {});
void DebugImGui_SetOnCameraRightChanged(const std::function<void(DirectX::XMFLOAT3 camera_right)>& callback = {});
void DebugImGui_SetOnCameraPresetApply(const std::function<void(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 front, DirectX::XMFLOAT3 up)>& callback = {});

void DebugImGui_CameraUpdate();

#endif // DEBUG_IMGUI_CAMERA_H
