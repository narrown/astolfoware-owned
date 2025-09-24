#pragma once
#include "SDK.h"
#include "DrawManager.h"

class CVisuals
{
public:
	void Run();
	void ProjectileTrajectory();
	void AspectRatio();
private:
	void DoDoubleTapVisual();
	void DoInformantionTab();
	void DoProjectilePath();
};

inline CVisuals gVisuals;