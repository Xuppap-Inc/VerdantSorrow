#pragma once
#include <vector>
#include "CollisionManager.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void init();
	//Método que hace update
	virtual void start();
	//Dibuja el fondo
	virtual void background(std::string backgroundName);
	virtual void playerGenerator(CollisionManager* colManager, Entity* player_);
protected:
	ecs::Manager* mngr_;
};

