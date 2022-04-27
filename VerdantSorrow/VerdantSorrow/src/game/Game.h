#pragma once
#include "../ecs/Manager.h"
#include "../game/SceneManager.h"
#include "../utils/Singleton.h"

class SceneManager;

class Game : public Singleton<Game>
{
	friend Singleton<Game>;
public:
	Game();
	~Game();
	void start();
	void update();
	void setExit(bool set);

	enum State {
		HUB = 1,
		FROGDEFEATED,
		TREEDEFEATED,
		FINALDEFEATED
	}state_;

private:
	ecs::Manager *mngr_;
	bool exit;
};

