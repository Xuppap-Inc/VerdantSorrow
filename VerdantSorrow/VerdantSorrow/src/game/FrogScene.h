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
	void waveSpawerGenerator(CollisionManager*& colManager);
	void background();
	void frogGenerator(CollisionManager* colManager, Entity* player_);

	void playerGenerator(CollisionManager* colManager, Entity* player_);
	
};

