/**
 * @file debug_imgui.h
 * @brief デバッグIMGUI
 */

#ifndef DEBUG_IMGUI_H
#define DEBUG_IMGUI_H

#include <Windows.h>
#include <d3d11.h>

void DebugImGui_Initialize(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void DebugImGui_Finalize(void);

void DebugImGui_Update();
void DebugImGui_Draw(void);

bool DebugImGui_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // DEBUG_IMGUI_H
