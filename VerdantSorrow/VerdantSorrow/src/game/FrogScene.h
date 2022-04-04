#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"

class CollisionChecker;
class FrogScene: public Scene
{
public: 
	FrogScene() :Scene() {};
	~FrogScene() {};
	void init();

	void update()override;
	void waveSpawerGenerator(CollisionManager*& colManager);
	void background();
	void frogGenerator(CollisionManager* colManager, Entity* player_);

	bool getAble();
	void setAble(bool a);
	void createLights();
private:
	bool isAble;
	Entity* player;
	Entity* Frog;
	CollisionChecker* colCheck_;
};

