#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"


class FinalBossScene: public Scene
{
public: 
	FinalBossScene() :Scene() {}
	~FinalBossScene() {};
	void init();
	void waveSpawnerGenerator(CollisionManager*& colManager);
	//start lo hace scene directamente
	void background();
	void finalBossGenerator(CollisionManager* colManager, Entity* player_);
	 
	void playerGenerator(CollisionManager* colManager, Entity* player_);
};

