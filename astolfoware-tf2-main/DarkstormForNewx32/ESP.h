#pragma once
#include "SDK.h"
#include "Utils.h"
class CESP
{
public:
	void Run();
private:
	void ESP();
	//void DoBuildingESPLoop();
	//void DoWorldESP();
	void DoBuildingESP();
	void DoWorld();
	void DoESPLoop();

};

inline CESP gESP;