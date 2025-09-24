#pragma once
#include "SDK.h"

class CChams {
private:
	void InitMaterial();
	IMaterial* CreateMaterial(const std::string_view name, const std::string_view shader, const std::string_view material) const;
	IMaterial* RegularMat;
	IMaterial* FlatMat;
	void SetColor(CBaseEntity* pEntity);
public:
	void Initialize();
};
inline CChams gChams;