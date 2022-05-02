#pragma once
#include <vector>
#include "CollisionManager.h"
#include "../sdlutils/SDLUtils.h"
class PlayerAttributes;
class CollideWithBorders;
class PlayerCtrl;
class FramedImage;
class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void init();

	//updates the scene we are into
	virtual void update();
	//Dibuja el fondo
	virtual void backgroundmovement(std::string backgroundName);
	virtual void background(std::string backgroundName,int height=sdlutils().height());
	virtual void playerGenerator(CollisionManager* colManager, Entity* player_);

	virtual void deactivateBoss() {};
	void checkPlayerFinishedDying();
	void checkPlayerDied();

protected:

	const int PLAYER_DEATH_DELAY = 5000;

	ecs::Entity* camera_;
	ecs::Manager* mngr_;

	PlayerAttributes* playerAttribs_;
	VirtualTimer* playerDeathTimer_;
	CollideWithBorders* bordersPlayer_;
	PlayerCtrl* playerCtrl_;

	Transform* playerTr_;
	FramedImage* playerImg_;

	bool playerDying = false;
};

