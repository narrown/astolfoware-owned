#include "ESP.h"
#include "Vars.h"
#include "EntityCache.h"
#include "Projectile.h"
#include "CreateMove.h"



void CESP::Run()
{
	if (!gLocalPlayer)
		return;
	/*
	for (int i = 1; i <= gInts.EntList->GetHighestEntityIndex(); i++)
	{
		if (!Vars::Misc::isThirdperson && i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity || !pLocal)
			continue;
		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive())
			continue;
		int nIndex = pEntity->GetIndex();
		bool bIsLocal = nIndex == gInts.Engine->GetLocalPlayer();
		if (!bIsLocal)
		{

			if (Vars::ESP::EnemyOnly && pEntity->GetTeamNum() == pLocal->GetTeamNum() && !gEntityCache.Friends[nIndex])
				continue;
		}

		ESP(pEntity);
		DoBuildingESP(pEntity);
		DoWorld(pEntity);
	}
	*/
	ESP();
	DoWorld();
	DoBuildingESP();
	//Nigga();
}
void CESP::DoWorld()
{
	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::WORLDESP))
	{
		switch (pEntity->GetClientClass()->iClassID)
		{
		case CCaptureFlag:
		{
			if (!pEntity)
			{
				return;
			}


			Vector vecScreen;
			Vector vecWorld;

			pEntity->GetWorldSpaceCenter(vecWorld);


			if (gDraw.WorldToScreen(vecWorld, vecScreen))
			{
				gDraw.DrawString(vecScreen.x, vecScreen.y, COLORCODE(255, 255, 255, 255), "INTEL");
			}
			break;
		}
		case CTFProjectile_Rocket:
		case CTFGrenadePipebombProjectile:
		case CTFProjectile_Jar:
		case CTFProjectile_JarGas:
		case CTFProjectile_JarMilk:
		case CTFProjectile_Arrow:
		case CTFProjectile_SentryRocket:
		case CTFProjectile_Flare:
		case CTFProjectile_GrapplingHook:
		case CTFProjectile_Cleaver:
		case CTFProjectile_EnergyBall:
		case CTFProjectile_EnergyRing:
		case CTFProjectile_HealingBolt:
		case CTFProjectile_ThrowableBreadMonster:
		{
			if (!pEntity)
			{
				return;
			}

			Vector vecScreen;
			Vector vecWorld;

			pEntity->GetWorldSpaceCenter(vecWorld);


			if (gDraw.WorldToScreen(vecWorld, vecScreen))
			{
				gDraw.DrawString(vecScreen.x, vecScreen.y, COLORCODE(255, 255, 255, 255), "*PROJECTILE*");
			}
		}
		break;
		}
	}
}
#include "DrawManager.h"
int GetPlayerColor(CBaseEntity* pPlayer)
{
	if (gEntityCache.Friends[pPlayer->GetIndex()])
	{
		return COLORCODE(0, 255, 0, 255);
	}
	if (pPlayer->GetIndex() == i::engine->GetLocalPlayer())
	{
		// 248, 0, 233
		return COLORCODE(248, 0, 233, 255);
	}
	switch (pPlayer->GetTeamNum())
	{
	case 2: //RED
		return COLORCODE(255, 0, 0, 255);
	case 3: //BLU
		// 30, 144, 255, 255
		return COLORCODE(30, 144, 255, 255);
	default:
		return COLORCODE(0, 153, 255, 255);
	}
	//return Color(0, 0, 0, 0); //no reason for this to be here, i just wanna look smart
}
#include "DrawManager.h"
int GetPlayerColorBuilding(CBaseEntity* pPlayer)
{

	switch (pPlayer->GetTeamNum())
	{
	case 2: //RED
		return COLORCODE(255, 0, 0, 255);
	case 3: //BLU
		return COLORCODE(30, 144, 255, 255);
	default:
		return COLORCODE(0, 153, 255, 255);
	}
	//return Color(0, 0, 0, 0); //no reason for this to be here, i just wanna look smart
}



void CESP::DoBuildingESP()
{
	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::BUILDINGS_ENEMIES))
	{
		if (!pEntity)
			continue;
		switch (pEntity->GetClientClass()->iClassID)
		{
		case CObjectSentrygun:
		{
			CTFObjectSentryGun* pSentryGun = reinterpret_cast<CTFObjectSentryGun*>(pEntity);
			if (!pSentryGun)
				continue;
			int teamcolor = GetPlayerColorBuilding(pSentryGun);

			const matrix3x4& vMatrix = pSentryGun->GetRgflCoordinateFrame();

			Vector vMin = pSentryGun->GetCollideableMins();
			Vector vMax = pSentryGun->GetCollideableMaxs();

			Vector vPointList[] = {
				Vector(vMin.x, vMin.y, vMin.z),
				Vector(vMin.x, vMax.y, vMin.z),
				Vector(vMax.x, vMax.y, vMin.z),
				Vector(vMax.x, vMin.y, vMin.z),
				Vector(vMax.x, vMax.y, vMax.z),
				Vector(vMin.x, vMax.y, vMax.z),
				Vector(vMin.x, vMin.y, vMax.z),
				Vector(vMax.x, vMin.y, vMax.z)
			};

			Vector vTransformed[8];

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 3; j++)
					vTransformed[i][j] = vPointList[i].Dot((Vector&)vMatrix[j]) + vMatrix[j][3];

			Vector flb, brt, blb, frt, frb, brb, blt, flt;

			if (!gDraw.WorldToScreen(vTransformed[3], flb) ||
				!gDraw.WorldToScreen(vTransformed[0], blb) ||
				!gDraw.WorldToScreen(vTransformed[2], frb) ||
				!gDraw.WorldToScreen(vTransformed[6], blt) ||
				!gDraw.WorldToScreen(vTransformed[5], brt) ||
				!gDraw.WorldToScreen(vTransformed[4], frt) ||
				!gDraw.WorldToScreen(vTransformed[1], brb) ||
				!gDraw.WorldToScreen(vTransformed[7], flt))
				continue;

			Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

			float left = flb.x;
			float top = flb.y;
			float right = flb.x;
			float bottom = flb.y;

			for (int i = 0; i < 8; i++)
			{
				if (left > arr[i].x)
					left = arr[i].x;
				if (top < arr[i].y)
					top = arr[i].y;
				if (right < arr[i].x)
					right = arr[i].x;
				if (bottom > arr[i].y)
					bottom = arr[i].y;
			}

			float x = left;
			float y = bottom;
			float w = right - left;
			float h = top - bottom;

			x += ((right - left) / 8); //pseudo fix for those THICC boxes
			w -= ((right - left) / 8) * 2;

			int iHp = pSentryGun->GetHealth(), iMaxHp = pSentryGun->GetMaxHealth();
			if (iHp > iMaxHp)
				iHp = iMaxHp;

			/* box */
			gDraw.OutlineRect(x - 1, y - 1, w + 2, h + 2, COLORCODE(0, 0, 0, 255));
			gDraw.OutlineRect(x, y, w, h, teamcolor);
			gDraw.OutlineRect(x + 1, y + 1, w - 2, h - 2, COLORCODE(0, 0, 0, 255));

			/* health bar */
			float flHealth = static_cast<float>(iHp);
			float flMaxHealth = static_cast<float>(iMaxHp);

			int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
			int nHeight2 = (h + 2);
			float ratio = (flHealth / flMaxHealth);
			gDraw.DrawRect(x - 6, y - 1, 4, h + 2, COLORCODE(0, 0, 0, 255));
			gDraw.DrawRect(x - 5, (y + nHeight - (nHeight * ratio)), 2, (nHeight * ratio) - 1, COLORCODE(0, 255, 0, 255));

			/* Text */
			int iY = 0;
			//gDraw.DrawString(x + w + 2, y + iY, teamcolor, "Sentry Gun");
			gDraw.DrawStringAligned(x + (w / 2),
				y - gDraw.GetESPHeight() - 2,
				teamcolor,
				TXT_CENTERX,
				Utils::ConvertUtf8ToWide("Sentry Gun").c_str());
			iY += gDraw.GetESPHeight();

			break;

		}
		case CObjectDispenser:
		{
			CObjectDispenserClass* pDispenser = reinterpret_cast<CObjectDispenserClass*>(pEntity);
			if (!pDispenser)
				continue;
			int teamcolor = GetPlayerColorBuilding(pDispenser);

			const matrix3x4& vMatrix = pDispenser->GetRgflCoordinateFrame();

			Vector vMin = pDispenser->GetCollideableMins();
			Vector vMax = pDispenser->GetCollideableMaxs();

			Vector vPointList[] = {
				Vector(vMin.x, vMin.y, vMin.z),
				Vector(vMin.x, vMax.y, vMin.z),
				Vector(vMax.x, vMax.y, vMin.z),
				Vector(vMax.x, vMin.y, vMin.z),
				Vector(vMax.x, vMax.y, vMax.z),
				Vector(vMin.x, vMax.y, vMax.z),
				Vector(vMin.x, vMin.y, vMax.z),
				Vector(vMax.x, vMin.y, vMax.z)
			};

			Vector vTransformed[8];

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 3; j++)
					vTransformed[i][j] = vPointList[i].Dot((Vector&)vMatrix[j]) + vMatrix[j][3];

			Vector flb, brt, blb, frt, frb, brb, blt, flt;

			if (!gDraw.WorldToScreen(vTransformed[3], flb) ||
				!gDraw.WorldToScreen(vTransformed[0], blb) ||
				!gDraw.WorldToScreen(vTransformed[2], frb) ||
				!gDraw.WorldToScreen(vTransformed[6], blt) ||
				!gDraw.WorldToScreen(vTransformed[5], brt) ||
				!gDraw.WorldToScreen(vTransformed[4], frt) ||
				!gDraw.WorldToScreen(vTransformed[1], brb) ||
				!gDraw.WorldToScreen(vTransformed[7], flt))
				continue;

			Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

			float left = flb.x;
			float top = flb.y;
			float right = flb.x;
			float bottom = flb.y;

			for (int i = 0; i < 8; i++)
			{
				if (left > arr[i].x)
					left = arr[i].x;
				if (top < arr[i].y)
					top = arr[i].y;
				if (right < arr[i].x)
					right = arr[i].x;
				if (bottom > arr[i].y)
					bottom = arr[i].y;
			}

			float x = left;
			float y = bottom;
			float w = right - left;
			float h = top - bottom;

			x += ((right - left) / 8); //pseudo fix for those THICC boxes
			w -= ((right - left) / 8) * 2;

			int iHp = pDispenser->GetHealth(), iMaxHp = pDispenser->GetMaxHealth();
			if (iHp > iMaxHp)
				iHp = iMaxHp;

			/* box */
			gDraw.OutlineRect(x - 1, y - 1, w + 2, h + 2, COLORCODE(0, 0, 0, 255));
			gDraw.OutlineRect(x, y, w, h, teamcolor);
			gDraw.OutlineRect(x + 1, y + 1, w - 2, h - 2, COLORCODE(0, 0, 0, 255));

			/* health bar */
			float flHealth = static_cast<float>(iHp);
			float flMaxHealth = static_cast<float>(iMaxHp);

			int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
			int nHeight2 = (h + 2);
			float ratio = (flHealth / flMaxHealth);
			gDraw.DrawRect(x - 6, y - 1, 4, h + 2, COLORCODE(0, 0, 0, 255));
			gDraw.DrawRect(x - 5, (y + nHeight - (nHeight * ratio)), 2, (nHeight* ratio) - 1, COLORCODE(0, 255, 0, 255));

			/* Text */
			int iY = 0;
			//gDraw.DrawString(x + w + 2, y + iY, teamcolor, "Dispenser");
			gDraw.DrawStringAligned(x + (w / 2),
				y - gDraw.GetESPHeight() - 2,
				teamcolor,
				TXT_CENTERX,
				Utils::ConvertUtf8ToWide("Dispenser").c_str());
			iY += gDraw.GetESPHeight();

			break;
		}
		break;
		}
	}
}

const char* GetPlayerName(CBaseEntity* pEntity, player_info_t& pInfo) {
	// this is bad... i think?
	if (Vars::Misc::StreamerMode) {
		if (pEntity->GetIndex() == i::engine->GetLocalPlayer())
		{
			return "Local";
		}
		if (gEntityCache.Friends[pEntity->GetIndex()]) {
			return "Friend";
		}
		else {
			return pInfo.name;
		}
	}
	else {
		return pInfo.name;
	}
}

#include "Rage.h"
#include "NccDraw.h"
void CESP::ESP()
{
	CBaseEntity* pLocalEntity = i::entlist->GetClientEntity(i::engine->GetLocalPlayer())->as<CBaseEntity>();
	if (!pLocalEntity || !i::engine->IsInGame())
		return;
	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::PLAYERS_ALL))
	{
		if (!pEntity)
			continue;
		if (pLocalEntity == NULL)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive())
			continue;
		int nIndex = pEntity->GetIndex();
		bool bIsLocal = nIndex == i::engine->GetLocalPlayer();
		if (!bIsLocal)
		{
			if (Vars::ESP::EnemyOnly && pEntity->GetTeamNum() == pLocalEntity->GetTeamNum() && !gEntityCache.Friends[nIndex])
				continue;
		}

		int CONDCOLOR = COLORCODE(255, 255, 0, 255);
		player_info_t pInfo;
		if (!i::engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
			continue;

		const matrix3x4& vMatrix = pEntity->GetRgflCoordinateFrame();

		Vector vMin = pEntity->GetCollideableMins();
		Vector vMax = pEntity->GetCollideableMaxs();

		Vector vPointList[] = {
			Vector(vMin.x, vMin.y, vMin.z),
			Vector(vMin.x, vMax.y, vMin.z),
			Vector(vMax.x, vMax.y, vMin.z),
			Vector(vMax.x, vMin.y, vMin.z),
			Vector(vMax.x, vMax.y, vMax.z),
			Vector(vMin.x, vMax.y, vMax.z),
			Vector(vMin.x, vMin.y, vMax.z),
			Vector(vMax.x, vMin.y, vMax.z)
		};

		Vector vTransformed[8];

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 3; j++)
				vTransformed[i][j] = vPointList[i].Dot((Vector&)vMatrix[j]) + vMatrix[j][3];

		Vector flb, brt, blb, frt, frb, brb, blt, flt;

		if (!gDraw.WorldToScreen(vTransformed[3], flb) ||
			!gDraw.WorldToScreen(vTransformed[0], blb) ||
			!gDraw.WorldToScreen(vTransformed[2], frb) ||
			!gDraw.WorldToScreen(vTransformed[6], blt) ||
			!gDraw.WorldToScreen(vTransformed[5], brt) ||
			!gDraw.WorldToScreen(vTransformed[4], frt) ||
			!gDraw.WorldToScreen(vTransformed[1], brb) ||
			!gDraw.WorldToScreen(vTransformed[7], flt))
			continue;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for (int i = 0; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;
			if (top < arr[i].y)
				top = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (bottom > arr[i].y)
				bottom = arr[i].y;
		}

		float x = left;
		float y = bottom;
		float w = right - left;
		float h = top - bottom;

		x += ((right - left) / 8); //pseudo fix for those THICC boxes
		w -= ((right - left) / 8) * 2;
		int clrPlayerCol = GetPlayerColor(pEntity);
		//Color clrBoneCol = gCheatMenu.esp_bones == 1 ? Color::White() : g.esp_bones == 2 ? Color::Green() : clrPlayerCol;
		int iY = 0;
		//iHp is only for health bar
		int iHp = pEntity->GetHealth(), iMaxHp = pEntity->GetMaxHealth();
		if (iHp > iMaxHp)
			iHp = iMaxHp;

		// box 
		if (Vars::ESP::Box)
		{
			gDraw.OutlineRect(x - 1, y - 1, w + 2, h + 2, COLORCODE(0, 0, 0, 255));
			gDraw.OutlineRect(x, y, w, h, clrPlayerCol);
			gDraw.OutlineRect(x + 1, y + 1, w - 2, h - 2, COLORCODE(0, 0, 0, 255));
		}

		if (Vars::ESP::Name)
		{
			
			
			gDraw.DrawStringAligned(x + (w / 2),
				y - gDraw.GetESPHeight() - 1,
				clrPlayerCol,
				TXT_CENTERX,
				Utils::ConvertUtf8ToWide(GetPlayerName(pEntity, pInfo)).c_str());
		}
		if (Vars::ESP::Healthbar)
		{
			//x -= 1;
			float flHealth = static_cast<float>(iHp);
			float flMaxHealth = static_cast<float>(iMaxHp);
			/*
			gDraw.DrawRect(x, y - 5, w, 3, COLORCODE(0, 0, 0, 150));
			int barWidth = (w / iMaxHp) * iHp;
			gDraw.DrawRect(x, y - 5, barWidth, 3, COLORCODE(0, 255, 0, 255));
			*/

			int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
			int nHeight2 = (h + 2);
			float ratio = (flHealth / flMaxHealth);
			gDraw.DrawRect(x - 6, y - 1, 4, h + 2, COLORCODE(0, 0, 0, 255));
			gDraw.DrawRect(x - 5, (y + nHeight - (nHeight * ratio)), 2, (nHeight * ratio) - 1, COLORCODE(0, 255, 0, 255));
			//gDraw.DrawRect(x - 5, y + (h - (h / iMaxHp * iHp)) + 1, 2, (h / iMaxHp * iHp) - 1, COLORCODE(0, 255, 0, 255));
			//x += 1;
		}

		if (Vars::ESP::Class)
		{
			gDraw.DrawString(x + w + 2, y + iY, clrPlayerCol, "%s", pEntity->szGetClass());
			iY += gDraw.GetESPHeight();
		}

		if (gMarkingSystem.IsMarked(pInfo.friendsID) != "")
		{
			gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "MARKED: %s", gMarkingSystem.IsMarked(pInfo.friendsID).c_str()); // draw the string asigned to that marked nigger
			iY += gDraw.GetESPHeight();
		}

		if (Vars::ESP::Conditions)
		{
			if (pEntity->GetCond() & TFCond_Cloaked)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*CLOAKED*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Bonked)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*BONKED*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Disguised)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*DISGUISED*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Healing)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*HEALING*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Ubercharged)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*UBERED*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_UberchargeFading)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*UBER FADING*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_MarkedForDeath)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*MARKED FOR DEATH*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Jarated)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*EW PISS*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_CritCola)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*CRIT COLA*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_MiniCrits)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*MINI CRITS*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Taunting)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*TAUNTING*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Zoomed)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*SCOPED*");
				iY += gDraw.GetESPHeight();
			}
			if (pEntity->GetCond() & TFCond_Slowed)
			{
				gDraw.DrawString(x + w + 2, y + iY, CONDCOLOR, "*SLOWED*");
				iY += gDraw.GetESPHeight();
			}
		}
	}
}