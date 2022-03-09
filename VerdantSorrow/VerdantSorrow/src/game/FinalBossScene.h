#pragma once

#include <vector>
#include "CollisionManager.h"
namespace ecs {
	class Manager;
}

class FinalBossScene
{
public: 
	FinalBossScene();
	virtual ~FinalBossScene();
	void init();
	void start();
	void background();
	void finalBossGenerator(CollisionManager* colManager, Entity* player_);

	void createHands(Transform* BossTr, CollisionManager* colManager);

	void playerGenerator(CollisionManager* colManager, Entity* player_);

	void platformGenerator(CollisionManager* colManager);
	
	void waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir);

	void flyGenerator(CollisionManager* colManager, Entity* player_);

private:
	ecs::Manager* mngr_;
};

