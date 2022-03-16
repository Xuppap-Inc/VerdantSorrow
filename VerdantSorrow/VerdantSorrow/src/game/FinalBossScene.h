#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"

namespace ecs {
	class Manager;
}

class FinalBossScene: public Scene
{
public: 
	FinalBossScene() :Scene() {}
	~FinalBossScene() {};
	void init();
	void start();
	void background();
	void finalBossGenerator(CollisionManager* colManager, Entity* player_);
	 
	void playerGenerator(CollisionManager* colManager, Entity* player_);

	
	void waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir);

	void flyGenerator(CollisionManager* colManager, Entity* player_);

private:
	ecs::Manager* mngr_;
};

