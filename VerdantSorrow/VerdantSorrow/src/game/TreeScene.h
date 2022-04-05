#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"

class CollisionChecker;
class TreeScene :public Scene
{
public:
	TreeScene() :Scene(), isAble(false) {}
	~TreeScene() {}
	void init();

	void update()override;
	void treeGenerator(CollisionManager* colManager);

	//void playerGenerator(CollisionManager* colManager);

	void rootGenerator(CollisionManager* colManager, float x);

	void lanternGenerator(CollisionManager* colManager, Entity* tree_, float x,float y);

	bool getAble();
	void setAble(bool a);

	void createLights();

private:
	Entity* player;
	Entity* tree_;
	bool isAble;
	CollisionChecker* colCheck_;
};

