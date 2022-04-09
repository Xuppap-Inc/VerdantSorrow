#pragma once
#include "../ecs/Manager.h"
#include "../game/SceneManager.h"

class SceneManager;

class Game
{
public:
	Game();
	~Game();
	void start();
	void update();
private:
	ecs::Manager *mngr_;

	int winHeight_;
	int winWidth_;
};