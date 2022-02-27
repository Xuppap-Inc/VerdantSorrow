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

	void frogGenerator(CollisionManager* colManager, Entity* player_);

	void playerGenerator(CollisionManager* colManager, Entity* player_);

	void platformGenerator(CollisionManager* colManager);
	
	void waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir);

	void flyGenerator(CollisionManager* colManager, Entity* player_);

private:
	ecs::Manager* mngr_;
};

