#include "EndScene.h"
#include "ImGui/imgui_impl_win32.h"
#include "Menu.h"
/*
static bool once = false;
static bool twice = false;
static bool bInitImGui = false;
void Nigga()
{
	if (!bInitImGui) {
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_Uint8);

		ImGuiStyle* style = &ImGui::GetStyle();
		auto& io = ImGui::GetIO();
		ImVec4* colors = ImGui::GetStyle().Colors;
		//ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\cour.ttf", 16.0f);
		colors[ImGuiCol_Text] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.56f, 0.41f, 0.04f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.04f, 0.05f, 0.08f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.2157f, 0.6549f, 1.0f, 0.2902f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.23f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.04f, 0.17f, 0.21f, 0.54f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.13f, 0.48f, 0.65f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.62f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 0.61f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 0.62f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.3059f, 0.7373f, 0.9765f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
		colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.10f, 0.20f);

		style->WindowPadding = ImVec2(3.00f, 2.00f);
		style->FramePadding = ImVec2(20.00f, 2.00f);
		style->CellPadding = ImVec2(6.00f, 0.00f);
		style->ItemSpacing = ImVec2(6.00f, 4.00f);
		style->ItemInnerSpacing = ImVec2(6.00f, 6.00f);
		style->TouchExtraPadding = ImVec2(0.00f, 0.00f);
		style->IndentSpacing = 0;
		style->ScrollbarSize = 1;
		style->GrabMinSize = 7;
		style->WindowBorderSize = 0;
		style->ChildBorderSize = 1;
		style->PopupBorderSize = 1;
		style->FrameBorderSize = 0;
		style->TabBorderSize = 1;
		style->WindowRounding = 0;
		style->ChildRounding = 0;
		style->FrameRounding = 0;
		style->PopupRounding = 0;
		style->ScrollbarRounding = 0;
		style->GrabRounding = 0;
		style->LogSliderDeadzone = 0;
		style->TabRounding = 0;
		style->Alpha = 0.80f;

		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style->SelectableTextAlign = ImVec2(0.0f, 0.5f);
		style->ButtonTextAlign = ImVec2(0.0f, 0.5f);

		style->DisplaySafeAreaPadding = ImVec2(0, 17);

		auto m_font_config = ImFontConfig();
		m_font_config.OversampleH = 1;
		m_font_config.OversampleV = 1;
		m_font_config.PixelSnapH = true;

		bInitImGui = true;
	}
}
HRESULT STDMETHODCALLTYPE Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	//init imgui related shit that only needs to be called once.
	if (!once)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(gInts.hWindow);
		ImGui_ImplDX9_Init(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
		//ImGui::DarkModernStyle();
		Nigga();
		once = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();

	ImGui::NewFrame();

	if (gMenu.IsOn)
	{
		gMenu.Initialize();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}

LRESULT STDMETHODCALLTYPE Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gMenu.IsOn = !gMenu.IsOn;

	if (gMenu.IsOn && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return TRUE;

	return CallWindowProc(gInts.WndProc, hWnd, uMsg, wParam, lParam);
}
*/