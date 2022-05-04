#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"

class CollisionChecker;
class TutorialScene : public Scene
{
public:
	TutorialScene() :Scene() {};
	~TutorialScene() {};
	void init();
	void background();
	void update()override;

	void changeSceneState(bool state) { changeScene_ = state; };
	void createFly(int x, int y);
	void createPlatform(int x, int y, int w, int h);
	void createDoor(int x, int y, int w, int h);
private:
	CollisionManager* colManager_;
	CollisionChecker* colCheck_;
	bool changeScene_;
};

