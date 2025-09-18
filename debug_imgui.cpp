/**
 * @file debug_imgui.h
 * @brief デバッグIMGUI
 */

#include "debug_imgui.h"

#include "lib/imgui.h"
#include "lib/imgui_impl_win32.h"
#include "lib/imgui_impl_dx11.h"

#include "debug_imgui_camera.h"

void DebugImGui_Initialize(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    ImGui_ImplWin32_EnableDpiAwareness();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pDevice, pContext);
}

void DebugImGui_Finalize()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void DebugImGui_Update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.5f;

    ImGui::SetNextWindowPos(ImVec2(16, 16));
    ImGui::Begin("Game Metrics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    DebugImGui_CameraUpdate();

    ImGui::End();
}

void DebugImGui_Draw()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool DebugImGui_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    return false;
}
