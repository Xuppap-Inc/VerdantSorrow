#pragma once

#include <vector>
#include "CollisionManager.h"
namespace ecs {
	class Manager;
}

class Game
{
public: 
	Game();
	virtual ~Game();
	void init();
	void start();

	void frogGenerator(CollisionManager* colManager);

	void playerGenerator(CollisionManager* colManager);

	void platformGenerator(CollisionManager* colManager);
	
private:
	ecs::Manager* mngr_;
};

