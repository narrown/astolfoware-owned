#pragma once
#include "Enums.h"
class IEngineVGUI
{
public:
	virtual					~IEngineVGUI(void) { }
	virtual unsigned int	GetPanel(EVGuiPanel type) = 0;
	virtual bool			IsGameUIVisible() = 0;
};