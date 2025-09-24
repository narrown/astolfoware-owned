#pragma once
#include "SDK.h"
#include <map>
#define RED(COLORCODE)	((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)	((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)	((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)	((int) COLORCODE & 0xFF )
#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))
#define COLORCODEForChams(r,g,b,a)((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

typedef struct CScreenSize_t
{
	int Height;
	int Width;

} CScreenSize;
#define TXT_DEFAULT		(1 << 0)
#define TXT_LEFT		(1 << 1)
#define TXT_TOP			(1 << 2)
#define TXT_CENTERX		(1 << 3)
#define TXT_CENTERY		(1 << 4)
#define TXT_CENTERXY	TXT_CENTERX | TXT_CENTERY

#define TEX_DEFAULT		(1 << 0)
#define TEX_LEFT		(1 << 1)
#define TEX_TOP			(1 << 2)
#define TEX_CENTERX		(1 << 3)
#define TEX_CENTERY		(1 << 4)
#define TEX_CENTERXY	TEX_CENTERX | TEX_CENTERY

//===================================================================================
class CDrawManager
{
public:
	unsigned long m_Font; // uh
	unsigned long m_Thoma;
	void UpdateScreenSize();
	void ReloadFonts();
	void Initialize();
	void DrawString(int x, int y, DWORD dwColor, const wchar_t* pszText);
	void DrawString(int x, int y, DWORD dwColor, const char* pszText, ...);
	void DrawStringCustomFont(unsigned long font, int x, int y, DWORD dwColor, const wchar_t* pszText);
	void DrawStringCustomFont(unsigned long font, int x, int y, DWORD dwColor, const char* pszText,  ...);
	void DrawStringAligned(int x, int y, DWORD dwColor, const short align, const wchar_t* pszText);
	void DrawStringAligned(int x, int y, DWORD dwColor, const short align, const char* pszText, ...);
	
	int GetESPHeight();
	int GetPixelTextSize(const char* pszText);
	int GetPixelTextSize(wchar_t* pszText);
	void DrawBox(Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius);
	void DrawRect(int x, int y, int w, int h, DWORD dwColor);
	void OutlineRect(int x, int y, int w, int h, DWORD dwColor);
	bool WorldToScreen(Vector& vOrigin, Vector& vScreen);
	void DrawLine(int x, int y, int x1, int y1, uint32_t Color);
	DWORD dwGetTeamColor(int iIndex)
	{
		static DWORD dwColors[] = { 0, //Dummy
					 0, // 1 Teamone (UNUSED)
					 0xFF8000FF, // 2 Teamtwo (RED)
					 0x0080FFFF, // 3 teamthree (BLUE)
					 0 // 4 Teamfour (UNUSED) 
		};
		return dwColors[iIndex];
	}
	void DrawGradientV(int x, int y, int w, int h, int c1, int c2);
	void GradientRect(const int x, const int y, const int x1, const int y1, const int& clrTop, const int& clrBottom, const bool bHorizontal);
	//std::map<uint64, int> m_mapAvatars = { };
	//void Avatar(const int x, const int y, const int w, const int h, const uint32 nFriendID);
	
};
inline CDrawManager gDraw;
inline CScreenSize gScreenSize;