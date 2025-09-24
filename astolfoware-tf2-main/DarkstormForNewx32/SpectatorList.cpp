#include "SpectatorList.h"
#include "EntityCache.h"
#include "Utils.h"

bool CSpectatorList::GetSpectators(CBaseEntity* pLocal)
{
	m_vecSpectators.clear();

	for (const auto& pTeammate : gEntityCache.GetGroup(EGroupType::PLAYERS_TEAMMATES))
	{
		C_BasePlayer* pObservedPlayer = i::entlist->GetClientEntityFromHandle(pTeammate->as<C_BasePlayer>()->ObserverTarget())->as<C_BasePlayer>();

		if (pTeammate && !pTeammate->IsAlive() && pObservedPlayer == pLocal)
		{
			std::wstring szMode = L"";

			switch (pTeammate->as<C_BasePlayer>()->ObserverMode()) {
			case OBS_MODE_FIRSTPERSON: { szMode = L"1st"; break; }
			case OBS_MODE_THIRDPERSON: { szMode = L"3rd"; break; }
			default: continue;
			}

			player_info_t PlayerInfo;
			if (i::engine->GetPlayerInfo(pTeammate->GetIndex(), &PlayerInfo))
				m_vecSpectators.push_back({ Utils::ConvertUtf8ToWide(PlayerInfo.name), szMode, gEntityCache.Friends[pTeammate->GetIndex()], pTeammate->GetTeamNum(), pTeammate->GetIndex() });
		}
	}

	return !m_vecSpectators.empty();
}

bool CSpectatorList::ShouldRun()
{
	return Vars::Misc::SpectatorList;
}

void CSpectatorList::Run()
{
	if (!ShouldRun())
		return;
	DrawClassic();
}

const wchar_t* FormatSpectatorInfo(const std::wstring& mode, const std::wstring& name) {
    static wchar_t buffer[256]; // Static buffer to hold the formatted string
    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"[%ls] %ls", mode.c_str(), name.c_str());
    return buffer;
}

#include "NccDraw.h"
#include "NccMath.h"
#include "InputSys.h"
#include "Menu.h"
void CSpectatorList::DrawClassic()
{
	if (const auto& pLocal = gEntityCache.m_pLocal)
	{
		if (!pLocal->IsAlive())
			return;

		int nDrawY = 50;

        static int m_x = ((gScreenSize.Height / 2) - 10); // middle of the screen
        static int m_y = 450;
        static bool m_dragging = false;
        static int m_dragoffsetx = 0, m_dragoffsety = 0;

        int cur_x = 0, cur_y = 0;
        int m_width = 200;
        int m_height = 20 + (m_vecSpectators.size() * 15);
        int t_x = m_x, t_y = m_y;
        bool IsOn = Menu::Get().IsVisible();

        // if (!IsOn)
         //    return;

        i::surface->GetCursorPosition(cur_x, cur_y);

        // Stop dragging when the left mouse button is released
        if (m_dragging && !InputSys::Get().IsKeyDown(VK_LBUTTON) || !IsOn)
            m_dragging = false;

        // If dragging, update the box's position
        if (m_dragging)
        {
            m_x = cur_x - m_dragoffsetx;
            m_y = cur_y - m_dragoffsety;

        }

        // Check if the cursor is within the draggable area and the left mouse button is pressed
        Vector2 tl(m_x, m_y);
        Vector2 br(tl.x + m_width, tl.y + m_height);
        if (!m_dragging && InputSys::Get().GetKeyState(VK_LBUTTON) == KeyState::Pressed)
        {
            if (Math::InBox(tl.x, tl.y, br.x, br.y, cur_x, cur_y))
            {
                m_dragging = true;
                m_dragoffsetx = cur_x - m_x;
                m_dragoffsety = cur_y - m_y;
            }
        }

        // Draw the box

        int textPadding = 5;
        Draw::FilledRectangle(tl.x, tl.y - 16, br.x, br.y, COLORCODE(248, 0, 233, 135, 255)); // pink
        Draw::Rectangle(tl.x, tl.y - 16, br.x, br.y, COLORCODE(248, 5, 233, 255)); // pink.
        Draw::Rectangle(tl.x + 1, tl.y - 15, br.x - 1, br.y - 1, COLORCODE(255, 5, 226, 255)); // pink
        tl += 6;
        br -= 6;
        Draw::FilledRectangle(tl.x, tl.y, br.x, br.y, COLORCODE(37, 36, 37, 255));
        Draw::Rectangle(tl, br, COLORCODE(50, 5, 5, 255)); // redish?
        Draw::Rectangle(tl.x - 1, tl.y - 1, br.x + 1, br.y + 1, COLORCODE(255, 5, 226, 255)); // pink 

        Draw::Text(tl.x + 3, tl.y - 17, L"Spectator List", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
        if (!GetSpectators(pLocal))
        {
            //Draw::Text(tl.x + textPadding, tl.y, L"No one is spectating.", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
            return;
        }
		for (const auto& Spectator : m_vecSpectators)
		{
			int nDrawX = gScreenSize.Width / 2;

			int w, h;
			//gInts.Surface->GetTextSize(g_Draw.m_vecFonts[FONT_ESP_NAME_OUTLINED].dwFont, (Spectator.m_sMode + Spectator.m_sName).c_str(), w, h);

			int nAddX = 0, nAddY = 0;
            Draw::Text(tl.x + 5, tl.y, FormatSpectatorInfo(Spectator.m_sMode.data(), 
                Spectator.m_sName.data()), Font::Get().MenuText, COLORCODE(255,255,255,255));

            /*
			gDraw.DrawString(
				nDrawX + nAddX, nDrawY,
				Spectator.m_bIsFriend ? COLORCODE(0, 255, 0, 255) : COLORCODE(255,255,255,255),
				"[%ls] %ls", Spectator.m_sMode.data(), Spectator.m_sName.data());
            */
            tl.y += 15;
		}
	}
}