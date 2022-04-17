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

	

	void createFly(int x, int y);
	void createPlatform(int x, int y, int w, int h);
	void createSpike();
private:
	CollisionManager* colManager_;
};

