#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"


class TreeScene :public Scene
{
public:
	TreeScene() :Scene() {}
	~TreeScene() {}
	void init();

	void treeGenerator(CollisionManager* colManager, Entity* player_);

	void playerGenerator(CollisionManager* colManager, Entity* player_);

	void rootGenerator(CollisionManager* colManager, Entity* player_, float x);

	void lanternGenerator(CollisionManager* colManager, Entity* tree_, float x,float y);


};

