#pragma once
#include "../ecs/Manager.h"

class Game
{
public:
	Game();
	~Game();
	void start();
	void update();
private:
	ecs::Manager *mngr_;
	SceneManager* scMngr_;
};

