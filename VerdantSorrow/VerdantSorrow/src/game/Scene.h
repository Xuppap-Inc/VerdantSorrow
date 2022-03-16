#pragma once
#include <vector>
#include "CollisionManager.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void init();
	virtual void start();
	virtual void background(std::string backgroundName);
	virtual void playerGenerator(CollisionManager* colManager, Entity* player_);
protected:
	ecs::Manager* mngr_;
};

