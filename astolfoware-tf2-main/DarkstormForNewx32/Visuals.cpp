#include "Visuals.h"
#include "Vars.h"
#include "Projectile.h"
#include "CritHack.h"
#include "NccDraw.h"
#include "GUI.h"
#include "NccUtilities.h"
#include "NccMath.h"
#include "Menu.h"
#include "Utils.h"
#include "ProjSim.h"
void CVisuals::Run()
{
	DoInformantionTab();
    //DoDoubleTapVisual();
	//DoCritVisual();
    DoProjectilePath();
}
#include "CreateMove.h"

void CVisuals::DoDoubleTapVisual() // not dt visual anymore but idrc
{
	int startX = ((gScreenSize.Width / 2) - 55); // midde of the screen.
	int startY = 500;
	int barLength = 75;
	int barHeight = 7;
	int MaxiumTicks = 23;
	int barWidth = (barLength * Vars::DT::Shifted) / MaxiumTicks;

	int centeredX = startX - (barLength / 2);
	int Bar = centeredX + 25;
	int centeredCommands = startX - 11;
	//gDraw.DrawRect(Bar + 1, startY + 1, barLength - 2, barHeight - 2, COLORCODE(0, 0, 0, 155)); // black; lil bit transparent

	//gDraw.DrawRect(Bar + 2, startY + 2, barWidth - 4, barHeight - 4, COLORCODE(112, 162, 78, 255)); // inside; greenish
	// H::Draw.DrawString(centeredCommands, startY - 15, COLORCODE(255, 255, 255, 255), "Stored Commands"); // Stored Comamnds Text.
    //         Draw::Text(tl.x + textPadding, tl.y + textPadding, L"Doubletap: ", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
    //if (!gInts.Engine->IsInGame())
    //   return;
    //Draw::Text(centeredCommands, startY - 15, CritStatus(), Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
}
void CVisuals::DoProjectilePath()
{
    if (!gLocalPlayer || !gProjectileAim.TargetVisual)
        return;
    if (!i::engine->IsConnected() || !i::engine->IsInGame())
        return;
    if (!GetAsyncKeyState(0x52))
        return;
    Vector PredictedPosition = gProjectileAim.FinalPositionLol;
    Vector vecPredictedScreen;
    if (gDraw.WorldToScreen(PredictedPosition, vecPredictedScreen))
    {
       // gDraw.DrawLine(vecPredictedScreen.x, vecPredictedScreen.y, vecPredictedScreen.x, vecPredictedScreen.y, COLORCODE(255, 255, 255, 255));
        Draw::Text(vecPredictedScreen.x, vecPredictedScreen.y, L"×", Font::Get().MenuText, COLORCODE(255,255,255,255));
    }
}
#include "Utils.h"
const wchar_t* GetDTStatus() 
{
    if (Vars::DT::Shifted == 21)
    {
        return L"Ready";
    }
    if (!Utils::CanPrimaryAttack(gLocalPlayer, gLocalWeapon))
    {
        return L"Wait..";
    }
    if (Vars::DT::Recharging)
    {
        return L"Recharging";
    }
    
    if (Vars::DT::Shifted < 21)
    {
        return L"Ticks Depleted!";
    }
    
    return L"Fuck.";
}
#undef max
int GetPingColor()
{
    int ms2 = std::max(0, static_cast<int>(std::round(m_Latency * 1000.f)));
    if (ms2 > 20 || ms2 < 20)
    {
        return COLORCODE(248, 0, 233, 135, 255);
    }
    if (ms2 > 90)
    {
        return COLORCODE(255, 255, 0, 255);
    }
    if (ms2 > 110)
    {
        return COLORCODE(255, 0, 0, 255);
    }
}

const wchar_t* GetPingStatus()
{
    int ms = std::max(0, static_cast<int>(std::round(m_Latency * 1000.f)));

    // Use a static buffer to return a wchar_t*
    static std::wstring pingStatus;
    pingStatus = std::to_wstring(ms) + L"ms";

    return pingStatus.c_str();
}

const wchar_t* GetFakelagTicks()
{
    CNetChannel* pNetChannel = reinterpret_cast<CNetChannel*>(i::engine->GetNetChannelInfo());
    if (!pNetChannel || !gLocalPlayer->IsAlive() || !gLocalPlayer)
        return L"The fuck?";
    //std::string nigger = std::to_string(pNetChannel->ChokedPackets);
    static std::wstring Choked;
    Choked = std::to_wstring(pNetChannel->ChokedPackets);
    return Choked.c_str();
}

int GetDTColor()
{
    if (Vars::DT::Shifted == 21)
    {
        return COLORCODE(248, 0, 233, 135, 255);
    }
    if (Vars::DT::Recharging)
    {
        return COLORCODE(255, 255, 0, 255); // yellow
    }
    if (Vars::DT::Shifted < 22)
    {
        return COLORCODE(255, 0, 0, 255); // red
    }
    if (Utils::CanPrimaryAttack(gLocalPlayer, gLocalWeapon)) {
        return COLORCODE(255, 255, 0, 255);
    }
    return COLORCODE(255, 0, 0, 255);
}
void CVisuals::AspectRatio() {
    static ConVar* AspectRatio = i::cvars->FindVar("r_aspectratio");
    AspectRatio->SetValue(Vars::Misc::AspectRatio);
}

void RenderLine(const Vector& v1, const Vector& v2, int c, bool bZBuffer)
{
    using FN = void(__cdecl*)(const Vector&, const Vector&, int, bool);
    static auto fnRenderLine = Memory::find_signature<FN>("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 85 F6");
    fnRenderLine(v1, v2, c, bZBuffer);
}
/* todo move these to signaturemanager */
/* although i doubt we are actually gonna use these.*/
void RenderBox(const Vector& vPos, const Vector& vMins, const Vector& vMaxs, const Vector& vOrientation, int cEdge, int cFace)
{
    {
        using FN = void(__cdecl*)(const Vector&, const Vector&, const Vector&, const Vector&, int, bool, bool);
        static auto fnRenderLine = Memory::find_signature<FN>("engine.dll", "55 8B EC 51 8B 45 ? 8B C8 FF 75"); //S::RenderBoxFace.As<FN>();
        fnRenderLine(vPos, vOrientation, vMins, vMaxs, cFace, false, false);
    }
    {
        using FN = void(__cdecl*)(const Vector&, const Vector&, const Vector&, const Vector&, int, bool);
        static auto fnRenderLine = Memory::find_signature<FN>("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 89 75 ? 85 F6 74 ? 8B 06 8B CE FF 50 ? A1");//S::RenderBoxEdge.As<FN>();
        fnRenderLine(vPos, vOrientation, vMins, vMaxs, cEdge, false);
    }
}

void CVisuals::ProjectileTrajectory() {
    if (!gLocalPlayer || !gLocalPlayer->IsAlive() || !gLocalWeapon)
        return;
    if (gLocalPlayer->GetClassNum() != TF2_Demoman)
        return;
    
    projectile_info_t projInfo = {};
    Vector view;
    i::engine->GetViewAngles(view);
    if (!gProjSim.GetInfo(gLocalPlayer, gLocalWeapon, view, projInfo, true))
        return;

    if (!gProjSim.Initialize(projInfo))
        return;

    for (int n = 0; n < TIME_TO_TICKS(projInfo.m_lifetime); n++)
    {
        Vector Old = gProjSim.GetOrigin();
        gProjSim.RunTick(projInfo);
        Vector New = gProjSim.GetOrigin();

        CGameTrace trace = {};
        CTraceFilterProjectile filter = {};
        //filter.pSkip = gLocalPlayer;
        Utils::TraceHull(Old, New, projInfo.m_hull * -1.f, projInfo.m_hull, MASK_SOLID, &filter, &trace);
        if (trace.fraction < 1)
        {
            Vector angles;
            VectorAngles(trace.plane.normal, angles);

            const Vector size = { projInfo.m_hull.x * 2.f, 16.f, 16.f };
            RenderBox(trace.endpos, size / -2, size / 2, angles, COLORCODE(255,255,255,255), COLORCODE(255,255,255,255));

            projInfo.PredictionLines.push_back({ trace.endpos, Math::GetRotatedPosition(trace.endpos, Math::VelocityToAngles(gProjSim.GetVelocity() * Vector(1, 1, 0)).Length2D() + 90, 12) });

            break;
        }
    }

    for (size_t i = 1; i < projInfo.PredictionLines.size(); i++)
        RenderLine(projInfo.PredictionLines.at(i - 1).first, projInfo.PredictionLines.at(i).first, COLORCODE(255, 255, 255, 255), false);
}
void CVisuals::DoInformantionTab()
{
    static int m_x = ((gScreenSize.Height / 2) - 50); // middle of the screen
    static int m_y = 450;
    static bool m_dragging = false;
    static int m_dragoffsetx = 0, m_dragoffsety = 0;

    int cur_x = 0, cur_y = 0;
    int m_width = 200, m_height = 100;
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

    Draw::Text(tl.x + 3, tl.y - 17, L"Information Box", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));

    

    if (!i::engine->IsInGame())
    {
       Draw::Text(tl.x + textPadding, tl.y + textPadding, L"You're not in game silly!", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
    }
    else /* DT ticks.*/
    {
        /* Doubletap */
        Draw::Text(tl.x + textPadding, tl.y + textPadding, L"Doubletap: ", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
        Draw::Text(tl.x + textPadding, tl.y + 15, GetDTStatus(), Font::Get().MenuText, GetDTColor());

        
        /* Ping */
        Draw::Text(tl.x + 5, tl.y + 25, L"Ping: ", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
        Draw::Text(tl.x + textPadding, tl.y + 35, GetPingStatus(), Font::Get().MenuText, GetPingColor());

        /* Fakelag */
        Draw::Text(tl.x + 5, tl.y + 45, L"Fakelag: ", Font::Get().MenuText, COLORCODE(255, 255, 255, 255));
        if (!Vars::FL::Fakelag)
            Draw::Text(tl.x + 5, tl.y + 55, L"Not Enabled.", Font::Get().MenuText, COLORCODE(255, 0, 0, 255));
        if (gLocalPlayer && !gLocalPlayer->IsAlive() && Vars::FL::Fakelag)
            Draw::Text(tl.x + 5, tl.y + 55, L"Unable To Fakelag.", Font::Get().MenuText, COLORCODE(255, 0, 0, 255));
       if (gLocalPlayer && gLocalPlayer->IsAlive() && Vars::FL::Fakelag)
            Draw::Text(tl.x + 5, tl.y + 55, GetFakelagTicks(), Font::Get().MenuText, COLORCODE(248, 0, 233, 135, 255));
    }
}