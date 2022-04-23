#pragma once
#include <vector>
#include "CollisionManager.h"
#include "../sdlutils/SDLUtils.h"
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
	virtual void backgroundmovement(std::string backgroundName);
	virtual void background(std::string backgroundName,int height=sdlutils().height());
	virtual void playerGenerator(CollisionManager* colManager, Entity* player_);
	virtual void playerGeneratorEscape(CollisionManager* colManager, Entity* player_);
protected:
	ecs::Entity* camera_;
	ecs::Manager* mngr_;
};

