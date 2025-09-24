#pragma once
#include "SDK.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui.h"

LRESULT STDMETHODCALLTYPE Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
typedef long(__stdcall* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(STDMETHODCALLTYPE* EndSceneFn)(IDirect3DDevice9*); //a bit ghetto
HRESULT STDMETHODCALLTYPE Hooked_EndScene(IDirect3DDevice9* pDevice);

inline EndSceneFn oEndScene;