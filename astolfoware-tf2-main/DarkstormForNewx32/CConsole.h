#pragma once
#include "SDK.h"

class CConsole
{
public:
    inline void OpenConsole()
    {
        AllocConsole();
        FILE* file;
        freopen_s(&file, "CONOUT$", "w", stdout); // Redirect stdout to the console
        SetConsoleTitleA("Astolfoware.dev | Vannie");
    }

    inline void CloseTheConsole()
    {
        HWND consoleWindow = GetConsoleWindow();
        if (consoleWindow != nullptr)
        {
            FreeConsole(); // Detach the console
            PostMessage(consoleWindow, WM_CLOSE, 0, 0); // Attempt to close the window
        }
    }
    inline void SetTextColor(int color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    inline void SetBackgroundColor(int backgroundColor)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);

        // Combine the new background color with the existing text attributes
        WORD newAttributes = (csbi.wAttributes & 0x0F) | (backgroundColor << 4);
        SetConsoleTextAttribute(hConsole, newAttributes);
    }
    inline void Print(const char* nigga)
    {
        std::cout << nigga;
    }
    inline void SetFont(const wchar_t* fontName, short fontSize)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Structure for console font information
        CONSOLE_FONT_INFOEX fontInfo = { 0 };
        fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX); // Set the structure size
        fontInfo.dwFontSize.Y = fontSize;             // Set the font height
        wcscpy_s(fontInfo.FaceName, fontName);        // Set the font name

        SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    }
};

inline CConsole* Console;