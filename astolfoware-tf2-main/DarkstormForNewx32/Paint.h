#pragma once
#include "SDK.h"

typedef void(__fastcall* PaintTraverse_t)(void*, void*, unsigned int, bool, bool);
inline PaintTraverse_t oPaintTraverse;
void __fastcall Hooked_PaintTraverse(void* pPanels, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);

void Intro();