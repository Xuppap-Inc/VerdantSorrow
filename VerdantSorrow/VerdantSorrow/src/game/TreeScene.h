#pragma once

#include <vector>
#include "CollisionManager.h"
namespace ecs {
	class Manager;
}

class TreeScene
{
public: 
	TreeScene();
	virtual ~TreeScene();
	void init();
	void start();

	void treeGenerator(CollisionManager* colManager, Entity* player_);

	void playerGenerator(CollisionManager* colManager, Entity* player_);

	void rootGenerator(CollisionManager* colManager, Entity* player_, float x);

	void lanternGenerator(CollisionManager* colManager, Entity* tree_, float x,float y);

private:
	ecs::Manager* mngr_;
};

