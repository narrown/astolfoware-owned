#include <Windows.h>
#include "Vector2.h"
#include "Fonts.h"
#include "DrawManager.h"

namespace Draw
{
	inline void Line(int x0, int y0, int x1, int y1, DWORD col)
	{
		i::surface->DrawSetColor(RED(col), GREEN(col), BLUE(col), ALPHA(col));
		i::surface->DrawLine(x0, y0, x1, y1);
	}

	inline void Line(Vector2 start_pos, Vector2 end_pos, DWORD col)
	{
		Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void Circle(Vector2 position, float points, float radius, DWORD color)
	{
		float step = (float)PI * 2.0f / points;

		for (float a = 0; a < (PI * 2.0f); a += step)
		{
			Vector2 start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
			Vector2 end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
			Line(start, end, color);
		}
	}

	inline void Circle(int x, int y, float points, float radius, DWORD color)
	{
		Circle(Vector2(x, y), points, radius, color);
	}

	inline void FilledRectangle(int x0, int y0, int x1, int y1, DWORD col)
	{
		i::surface->DrawSetColor(RED(col), GREEN(col), BLUE(col), ALPHA(col));
		i::surface->DrawFilledRect(x0, y0, x1, y1);
	}

	inline void FilledRectangle(Vector2 start_pos, Vector2 end_pos, DWORD col)
	{
		FilledRectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void Rectangle(int x0, int y0, int x1, int y1, DWORD col)
	{
		i::surface->DrawSetColor(RED(col), GREEN(col), BLUE(col), ALPHA(col));
		i::surface->DrawOutlinedRect(x0, y0, x1, y1);
	}

	inline void Rectangle(Vector2 start_pos, Vector2 end_pos, DWORD col)
	{
		Rectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void FilledGradientRectangle(int x0, int y0, int x1, int y1, DWORD top, DWORD bottom)
	{
		i::surface->DrawSetColor(RED(top), GREEN(top), BLUE(top), ALPHA(top));
		i::surface->DrawFilledRectFade(x0, y0, x1, y1, 255, 255, false);

		i::surface->DrawSetColor(RED(bottom), GREEN(bottom), BLUE(bottom), ALPHA(bottom));
		i::surface->DrawFilledRectFade(x0, y0, x1, y1, 0, 255, false);
	}

	inline void FilledGradientRectangle(Vector2 pos, Vector2 size, DWORD top, DWORD bottom)
	{
		FilledGradientRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y, top, bottom);
	}

	inline void Text(int x, int y, const wchar_t* text, HFont font, DWORD color, bool centeredx, bool centeredy)
	{
		if (centeredx || centeredy)
		{
			int wide, tall;
			i::surface->GetTextSize(font, text, wide, tall);
			if (centeredx)
				x -= wide / 2;
			if (centeredy)
				y -= tall / 2;
		}

		i::surface->DrawSetTextPos(x, y);
		i::surface->DrawSetTextFont(font);
		i::surface->DrawSetTextColor(RED(color), GREEN(color), BLUE(color), ALPHA(color));
		i::surface->DrawPrintText(text, wcslen(text));
	}

	inline void Text(int x, int y, const wchar_t* text, HFont font, DWORD color, bool centered = false)
	{
		Text(x, y, text, font, color, centered, centered);
	}

	inline Vector2 GetTextSize(const wchar_t* text, HFont font)
	{
		int x_res, y_res;
		i::surface->GetTextSize(font, text, x_res, y_res);
		return Vector2(x_res, y_res);
	}
}