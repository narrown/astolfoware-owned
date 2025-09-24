#include "Menu.h"
#include "Vars.h"
#include <format>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Utils.h"
#include <comdef.h>  // you will need this

#include "Rage.h"
#include "CreateMove.h"
void Rage(int friendsid)
{
	gMarkingSystem.LoadFromFile();
	gMarkingSystem.Save(friendsid, "RAGE");
	gMarkingSystem.Flush();
}
bool     m_PlayerList = false;
void Menu::Init()
{
	m_isVisible = false;

	CWindow Main(100, 100, 700, 400);

	auto AimbotTab = new CChild(0, 0, 220, L"Hitscan");
	{
		AimbotTab->AddControl(new CCheckBox(L"Enabled", &Vars::Hitscan::bEnable));
		AimbotTab->AddControl(new CCheckBox(L"Silent", &Vars::Hitscan::bSilentAim));
		AimbotTab->AddControl(new CSlider<int>(L"Field of View", 0, 180, &Vars::Hitscan::iFov));
		AimbotTab->AddControl(new CCheckBox(L"Autoshoot", &Vars::Hitscan::bAutoShoot));
		AimbotTab->AddControl(new CCheckBox(L"Multi-Point", &Vars::Hitscan::MultiPointIfNoHitboxesAreVisible));
		AimbotTab->AddControl(new CSlider<int>(L"Max Players", 0, 10, &Vars::Hitscan::MaxPlayers));
		AimbotTab->AddControl(new CCombo(&Vars::Hitscan::Key, { L"(Hitscan Key)", L"Mouse 1", L"Mouse 2", L"Mouse 3", L"Mouse 4", L"Mouse 5", L"Shift", L"Alt", L"R", L"CTRL", L"F" }));
		AimbotTab->AddControl(new CCombo(&Vars::Hitscan::iHitbox, { L"Auto", L"Head", L"Body", L"Pelvis"}));
	}
	Main.AddControl(AimbotTab);

	auto DTTab = new CChild(230, 0, 220, L"DT");
	{
		DTTab->AddControl(new CCheckBox(L"Enable DT", &Vars::DT::DoubleTap));
		DTTab->AddControl(new CCombo(&Vars::DT::Key, { L"(Recharge Key)", L"Mouse 1", L"Mouse 2", L"Mouse 3", L"Mouse 4", L"Mouse 5", L"Shift", L"Alt", L"R", L"CTRL", L"F" }));
		DTTab->AddControl(new CCheckBox(L"Anti-Warp", &Vars::DT::AntiWarp));
	}
	Main.AddControl(DTTab);

	auto MiscTab = new CChild(230, 60, 220, L"Movement");
	{
		MiscTab->AddControl(new CCheckBox(L"BunnyHop", &Vars::Misc::Bhop));
		MiscTab->AddControl(new CCheckBox(L"Directional AutoStrafe", &Vars::Misc::AutoStrafe));
		MiscTab->AddControl(new CCheckBox(L"ThirdPerson", &Vars::Misc::ThirdPerson));
		MiscTab->AddControl(new CCheckBox(L"Crit Hax", &Vars::CritHax::Enable));
		MiscTab->AddControl(new CCombo(&Vars::CritHax::CritKey, { L"(Crit Key)", L"Mouse 1", L"Mouse 2", L"Mouse 3", L"Mouse 4", L"Mouse 5", L"Shift", L"Alt", L"R", L"CTRL", L"F" }));
	}
	Main.AddControl(MiscTab);

	auto ProjectileAimTab = new CChild(230, 160, 220, L"Projectile");
	{
		ProjectileAimTab->AddControl(new CCheckBox(L"Enabled", &Vars::ProjectileAim::Enable));
		ProjectileAimTab->AddControl(new CCheckBox(L"Silent", &Vars::ProjectileAim::Silent));
		ProjectileAimTab->AddControl(new CCheckBox(L"AutoShoot", &Vars::ProjectileAim::AutoShoot));
		ProjectileAimTab->AddControl(new CSlider<int>(L"FOV", 0, 180, &Vars::ProjectileAim::AimFOV));
		ProjectileAimTab->AddControl(new CSlider<int>(L"Max Players", 0, 10, &Vars::ProjectileAim::MaxPlayers));
		ProjectileAimTab->AddControl(new CSlider<float>(L"Max Simulation Time", 0, 10, &Vars::ProjectileAim::SimulationTime));
		ProjectileAimTab->AddControl(new CSlider<int>(L"Strafe Chance", 0, 100, &Vars::ProjectileAim::StrafeChance));
		ProjectileAimTab->AddControl(new CCombo(&Vars::ProjectileAim::Key, { L"(ProjectileAim Key)", L"Mouse 1", L"Mouse 2", L"Mouse 3", L"Mouse 4", L"Mouse 5", L"Shift", L"Alt", L"R", L"CTRL", L"F" }));

		//ProjectileAimTab->AddControl(new CSlider<int>(L"Strafe Smoothness", 0, 100, &Vars::ProjectileAim::StrafeSmooth));
	}
	Main.AddControl(ProjectileAimTab);

	auto ESPTab = new CChild(0, 150, 220, L"ESP");
	{
		ESPTab->AddControl(new CCheckBox(L"Enabled", &Vars::ESP::Enable));
		ESPTab->AddControl(new CCheckBox(L"Enemy Only", &Vars::ESP::EnemyOnly));
		ESPTab->AddControl(new CCheckBox(L"Box", &Vars::ESP::Box));
		ESPTab->AddControl(new CCheckBox(L"Healthbar", &Vars::ESP::Healthbar));
		ESPTab->AddControl(new CCheckBox(L"Name", &Vars::ESP::Name));
		ESPTab->AddControl(new CCheckBox(L"Class", &Vars::ESP::Class));
		ESPTab->AddControl(new CCheckBox(L"Conditions", &Vars::ESP::Conditions));
		ESPTab->AddControl(new CCheckBox(L"Chams", &Vars::ESP::Chams));
	}
	Main.AddControl(ESPTab);
	
	auto MiscAgainTab = new CChild(460, 0, 220, L"Ignores");
	{
		MiscAgainTab->AddControl(new CCheckBox(L"Ignore Friends", &Vars::PriorityOptions::IgnoreFriends));
		MiscAgainTab->AddControl(new CCheckBox(L"Ignore Cloaked", &Vars::PriorityOptions::IgnoreCloaked));
	}
	Main.AddControl(MiscAgainTab);
	auto MiscAgainTabAgain = new CChild(460, 55, 220, L"Misc");
	{
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Noisemaker", &Vars::Misc::NoiseMaker));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Remove Scope", &Vars::Removals::RemoveScope));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Remove Zoom", &Vars::Removals::RemoveZoom));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Anti-Autobalance", &Vars::Misc::AntiAutobalance));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Skip Welcome", &Vars::Misc::SkipWelcome));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Auto Detonate", &Vars::Misc::AutoDetonate));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Auto Backstab", &Vars::Misc::AutoBackstab));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"sv_pure Bypass", &Vars::Misc::PureBypass));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Spectator List", &Vars::Misc::SpectatorList));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Streamer Mode", &Vars::Misc::StreamerMode));
		// hvh
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Anti-Aim", &Vars::AA::AA));
		MiscAgainTabAgain->AddControl(new CCombo(&Vars::AA::Pitch, { L"Disabled", L"Up", L"Down", L"Random"}));
		MiscAgainTabAgain->AddControl(new CCombo(&Vars::AA::FakePitch, { L"Disabled", L"Fake Up", L"Fake Down", L"Fake Random"}));
		MiscAgainTabAgain->AddControl(new CCombo(&Vars::AA::Yaw, { L"Disabled", L"Right", L"Left", L"Back", L"Emotion", 
			L"Random", L"Fake Sideways Right", L"Fake Sideways Left", L"Fake Right", L"Half Back Left", L"Half Back Right",
			L"Fake Left", L"Fake Manual"}));
		MiscAgainTabAgain->AddControl(new CCheckBox(L"Fakelag", &Vars::FL::Fakelag));
		MiscAgainTabAgain->AddControl(new CSlider<int>(L"Fakelag Amount", 0, 22, &Vars::FL::Amount));
		MiscAgainTabAgain->AddControl(new CSlider<int>(L"FOV", 0, 125, &Vars::Misc::FOV));
		MiscAgainTabAgain->AddControl(new CSlider<float>(L"Aspect Ratio", 0, 2, &Vars::Misc::AspectRatio));
		MiscAgainTabAgain->AddControl(new CButton(10, 10, L"Reload MarkedList", []() { gMarkingSystem.LoadFromFile(); }));
	}
	Main.AddControl(MiscAgainTabAgain);
	MenuForm.AddWindow(Main);
}

void Menu::Kill()
{}

void Menu::PaintTraverse()
{
	if (m_isVisible)
		MenuForm.Paint();
}

void Menu::Click()
{
	if (m_isVisible)
		MenuForm.Click();
}

void Menu::HandleInput(WPARAM vk)
{
	if (m_isVisible)
		MenuForm.HandleInput(vk);
}

void Menu::Toggle()
{
	m_isVisible = !m_isVisible;
}