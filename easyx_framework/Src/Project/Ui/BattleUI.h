#pragma once
#include "UserInterface.h"



class BattleUI :public UserInterface
{
	Image* SeedBank;
	HorizontalPanel* Panel;
	Text* SunShine;
public:
	BattleUI();

	void On_Update(float deltaTime);
};