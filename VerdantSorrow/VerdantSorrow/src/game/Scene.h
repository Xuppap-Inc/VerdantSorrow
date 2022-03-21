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
	//virtual void start();
	//updates the scene we are into
	virtual void update();
	//Dibuja el fondo
	virtual void background(std::string backgroundName);
	virtual void playerGenerator(CollisionManager* colManager, Entity* player_);
protected:
	ecs::Manager* mngr_;
};

