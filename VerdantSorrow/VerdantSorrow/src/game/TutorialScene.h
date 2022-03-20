#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"


class TutorialScene : public Scene
{
public:
	TutorialScene() :Scene() {};
	~TutorialScene() {};
	void init();
	void background();

	void playerGenerator(CollisionManager* colManager, Entity* player_);

	ecs::Entity* createFly(int x, int y);
	ecs::Entity* createPlatform(int x, int y, int w, int h);
private:
	CollisionManager* colManager_;
};

