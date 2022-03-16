#pragma once
#include <vector>
#include "CollisionManager.h"

namespace ecs {
	class Manager;
}
class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void init();
	virtual void start();
	virtual void background();
	virtual void playerGenerator(CollisionManager* colManager, Entity* player_);
protected:
	ecs::Manager* mngr_;
};

