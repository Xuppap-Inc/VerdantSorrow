#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"


class FrogScene: public Scene
{
public: 
	FrogScene() :Scene() {};
	~FrogScene() {};
	void init();
	void background();
	void frogGenerator(CollisionManager* colManager, Entity* player_);

	void playerGenerator(CollisionManager* colManager, Entity* player_);

	void platformGenerator(CollisionManager* colManager);
	
	void waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir);

	void flyGenerator(CollisionManager* colManager, Entity* player_);

};

