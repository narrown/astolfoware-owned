#include "Chams.h"
#include "EntityCache.h"
#include "Vars.h"
#include "CreateMove.h"
/*
Credit: Drev
*/

static bool InitalizedMats = false;


IMaterial* CChams::CreateMaterial(const std::string_view name, const std::string_view shader, const std::string_view material) const {
	KeyValues* Values = new KeyValues(shader.data());
	Values->LoadFromBuffer(name.data(), material.data());
	return i::materialsystem->CreateMaterialSigged(name.data(), Values);
}

void CChams::InitMaterial() {
	RegularMat = CreateMaterial(
		("astolfoware_regular.vmt"),
		("VertexLitGeneric"),
		(
			R"#("VertexLitGeneric" {
					"$basetexture" "vgui/white_additive"
					"$ignorez"      "0"
					"$model"		"1"
					"$flat"			"0"
					"$nocull"		"1"
					"$halflambert"	"1"
					"$nofog"		"1"
					"$wireframe"	"0"
				})#"
			)
	);
	FlatMat = CreateMaterial(
		("astolfoware_mettallic.vmt"),
		("VertexLitGeneric"),
		(
			R"#("VertexLitGeneric"
            {
                    "$basetexture"                "vgui/white_additive"
                    "$ignorez"                    "0"
                    "$phong"                    "1"
                    "$BasemapAlphaPhongMask"    "1"
                    "$phongexponent"            "15"
                    "$normalmapalphaenvmask"    "1"
                    "$envmap"                    "env_cubemap"
                    "$envmaptint"                "[0.0 0.0 0.0]"
                    "$phongboost"                "[0.6 0.6 0.6]"
                    "phongfresnelranges"        "[0.5 0.5 1.0]"
                    "$nofog"                    "1"
                    "$model"                    "1"
                    "$nocull"                    "0"
                    "$selfillum"                "1"
                    "$halflambert"                "1"
                    "$znearer"                    "0"
                    "$flat"                        "0"
                    "$rimlight"                    "1"
                    "$rimlightexponent"            "2"
                    "$rimlightboost"            "0"
            }
            )#"
			)
	);
	
	if (!RegularMat || RegularMat->IsErrorMaterial())
		return;
	if (!FlatMat || FlatMat->IsErrorMaterial())
		return;

	RegularMat->IncrementReferenceCount();
	FlatMat->IncrementReferenceCount();

	InitalizedMats = true;
}

#define STUDIO_RENDER 0x00000001


static SColor colors_team[] =
{
	SColor(0), // 0 aka 0
	SColor(0, 255, 0), // 1 aka friends
	SColor(255, 0, 0), // 2 Red
	SColor(0, 128, 255), // 3 Blu
	SColor(0), // 4 Teamfour (UNUSED) 
	SColor(248, 0, 233) // 5
};

int Color(CBaseEntity* pEntity) {
	if (gEntityCache.Friends[pEntity->GetIndex()])
	{
		return 1;
	}
	if (pEntity->GetIndex() == i::engine->GetLocalPlayer()) {
		return 5;
	}
	switch (pEntity->GetTeamNum())
	{
	case 2: //RED
		return 2;
	case 3: //BLU
		// 30, 144, 255, 255
		return 3;
	default:
		return 4;
	}
}


void CChams::SetColor(CBaseEntity* pEntity) {
	byte team = Color(pEntity);
	SColor color = colors_team[team];
	float blend[3] = { (float)color[0] / 255.f, (float)color[1] / 255.f, (float)color[2] / 255.f };
	i::renderview->SetBlend(255.f);
	i::renderview->SetColorModulation(blend);
}
/* todo do this on drawmodel or drawmodelexecute */
void CChams::Initialize() {
	if (!Vars::ESP::Chams)
		return;
	if (!InitalizedMats)
		InitMaterial();
	
	for (const auto& pEntity : gEntityCache.GetGroup(EGroupType::PLAYERS_ALL)) {
		if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;

		if (!RegularMat) 
			continue;
		int nIndex = pEntity->GetIndex();
		bool bIsLocal = nIndex == i::engine->GetLocalPlayer();
		if (!bIsLocal)
		{
			if (Vars::ESP::EnemyOnly && pEntity->GetTeamNum() == gLocalPlayer->GetTeamNum() && !gEntityCache.Friends[nIndex])
				continue;
		}
		
		SetColor(pEntity);

		FlatMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		i::modelrender->ForcedMaterialOverride(FlatMat); 
		pEntity->DrawModel(STUDIO_RENDER); 

		RegularMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
		i::modelrender->ForcedMaterialOverride(RegularMat);
		pEntity->DrawModel(STUDIO_RENDER); 

		i::modelrender->ForcedMaterialOverride(nullptr); // done
	}
}