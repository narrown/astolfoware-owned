#include "DrawManager.h"
#pragma warning(disable : 4244) // SHUSH NIGGER LOL
#define ESP_HEIGHT 12

void CDrawManager::UpdateScreenSize()
{
	i::engine->GetScreenSize(gScreenSize.Width, gScreenSize.Height);
	//this->c = (this->w / 2);
}
/*
namespace OnScreenSizeChanged
	{
		const int index = 111;
		using fn = void(__thiscall *)(CSurface *, int, int);
		void __stdcall Hook(int OldWidht, int OldHeight);
	}

	surface
*/
void CDrawManager::ReloadFonts()
{
	/* first empty them */
	m_Font = 0x0;
	m_Thoma = 0x0;

	m_Font = i::surface->CreateFont();
	m_Thoma = i::surface->CreateFont();
	i::surface->SetFontGlyphSet(m_Font, "Verdana", ESP_HEIGHT, 800, 0, 0, 0x200);
	i::surface->SetFontGlyphSet(m_Thoma, "Tahoma", ESP_HEIGHT, 800, 0, 0, 0x200);
}
//===================================================================================
void CDrawManager::Initialize()
{
	if (i::surface == NULL)
		return;

	//gInts.Engine->GetScreenSize(gScreenSize.Width, gScreenSize.Height);
	UpdateScreenSize();
	m_Font = i::surface->CreateFont();
	m_Thoma = i::surface->CreateFont();
	i::surface->SetFontGlyphSet(m_Font, "Verdana", ESP_HEIGHT, 800, 0, 0, 0x200);
	i::surface->SetFontGlyphSet(m_Thoma, "Tahoma", ESP_HEIGHT, 800, 0, 0, 0x200);
}
//===================================================================================
void CDrawManager::DrawString(int x, int y, DWORD dwColor, const wchar_t* pszText)
{
	if (pszText == NULL)
		return;

	i::surface->DrawSetTextPos(x, y);
	i::surface->DrawSetTextFont(m_Font);
	i::surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawPrintText(pszText, wcslen(pszText));
}

void CDrawManager::DrawStringAligned(int x, int y, DWORD dwColor, const short align, const wchar_t* pszText)
{
	if (pszText == NULL)
		return;

	if (align)
	{
		int w = 0, h = 0;
		i::surface->GetTextSize(m_Font, pszText, w, h);

		if (align & TXT_LEFT)
			x -= w;

		if (align & TXT_TOP)
			y -= h;

		if (align & TXT_CENTERX)
			x -= (w / 2);

		if (align & TXT_CENTERY)
			y -= (h / 2);
	}

	i::surface->DrawSetTextPos(x, y);
	i::surface->DrawSetTextFont(m_Font);
	i::surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawPrintText(pszText, wcslen(pszText));
}
void CDrawManager::DrawStringAligned(int x, int y, DWORD dwColor, const short align, const char* pszText, ...)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);

	if (align)
	{
		int w = 0, h = 0;
		i::surface->GetTextSize(m_Font, szString, w, h);

		if (align & TXT_LEFT)
			x -= w;

		if (align & TXT_TOP)
			y -= h;

		if (align & TXT_CENTERX)
			x -= (w / 2);

		if (align & TXT_CENTERY)
			y -= (h / 2);
	}

	i::surface->DrawSetTextPos(x, y);
	i::surface->DrawSetTextFont(m_Font);
	i::surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawPrintText(szString, wcslen(szString));
}
//===================================================================================
void CDrawManager::DrawString(int x, int y, DWORD dwColor, const char* pszText, ...)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);


	i::surface->DrawSetTextPos(x, y);
	i::surface->DrawSetTextFont(m_Font);
	i::surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawPrintText(szString, wcslen(szString));
}
void CDrawManager::DrawStringCustomFont(unsigned long font, int x, int y, DWORD dwColor, const char* pszText,  ...)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);


	i::surface->DrawSetTextPos(x, y);
	i::surface->DrawSetTextFont(font);
	i::surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawPrintText(szString, wcslen(szString));
}
void CDrawManager::DrawStringCustomFont(unsigned long font, int x, int y, DWORD dwColor, const wchar_t* pszText)
{
	if (pszText == NULL)
		return;

	i::surface->DrawSetTextPos(x, y);
	i::surface->DrawSetTextFont(font);
	i::surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawPrintText(pszText, wcslen(pszText));
}
//===================================================================================
int CDrawManager::GetESPHeight()
{
	return ESP_HEIGHT;
}
//===================================================================================
void CDrawManager::DrawRect(int x, int y, int w, int h, DWORD dwColor)
{
	i::surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawFilledRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::OutlineRect(int x, int y, int w, int h, DWORD dwColor)
{
	i::surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	i::surface->DrawOutlinedRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::DrawBox(Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius)
{
	Vector vScreen;

	if (!WorldToScreen(vOrigin, vScreen))
		return;

	int radius2 = radius << 1;

	OutlineRect(vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, 0x000000FF);
	OutlineRect(vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + (box_width + 2), radius2 + (box_width + 2), 0x000000FF);
	DrawRect(vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x - radius, vScreen.y + radius, radius2, box_width, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x - radius, vScreen.y - radius, box_width, radius2, COLORCODE(r, g, b, alpha));
	DrawRect(vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, COLORCODE(r, g, b, alpha));
}
#include "Vars.h"
//===================================================================================
bool CDrawManager::WorldToScreen(Vector& vOrigin, Vector& vScreen)
{
	/*
	const matrix3x4& worldToScreen = gInts.Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		vScreen.x = (gScreenSize.Width / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.Width + 0.5); //Get the X dimension and push it in to the Vector.
		vScreen.y = (gScreenSize.Height / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.Height + 0.5); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
	*/
	const matrix3x4& worldToScreen = Vars::Misc::m_WorldToProjection.As3x4();

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];
	vScreen.z = 0;

	if (w > 0.001)
	{
		float fl1DBw = 1 / w;
		vScreen.x = (gScreenSize.Width / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.Width + 0.5);
		vScreen.y = (gScreenSize.Height / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.Height + 0.5);
		return true;
	}

	return false;
}
void CDrawManager::DrawLine(int x, int y, int x1, int y1, uint32_t Color)
{
	i::surface->DrawSetColor(RED(Color), GREEN(Color), BLUE(Color), ALPHA(Color));
	i::surface->DrawLine(x, y, x1, y1);
}

void CDrawManager::DrawGradientV(int x, int y, int w, int h, int c1, int c2)
{
	DrawRect(x, y, w, h, c1);
	BYTE first = RED(c2);
	BYTE second = GREEN(c2);
	BYTE third = BLUE(c2);
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		DrawRect(x, y + i, w, 1, COLORCODE(first, second, third, ia));
	}
}


void CDrawManager::GradientRect(const int x, const int y, const int x1, const int y1, const int& clrTop, const int& clrBottom, const bool bHorizontal)
{
	i::surface->DrawSetColor(RED(clrTop), GREEN(clrTop), BLUE(clrTop), ALPHA(clrTop));
	i::surface->DrawFilledRectFade(x, y, x1, y1, 255u, 255u, bHorizontal);

	i::surface->DrawSetColor(RED(clrBottom), GREEN(clrBottom), BLUE(clrBottom), ALPHA(clrBottom));
	i::surface->DrawFilledRectFade(x, y, x1, y1, 0u, 255u, bHorizontal);
}
