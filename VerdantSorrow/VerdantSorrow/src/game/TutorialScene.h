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
	void update()override;

	void playerGenerator(CollisionManager* colManager, Entity* player_);

};

