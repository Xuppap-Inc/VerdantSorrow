#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"


class TreeScene :public Scene
{
public:
	TreeScene() :Scene() {}
	~TreeScene() {}
	void init();

	void update()override;
	void treeGenerator(CollisionManager* colManager);

	void playerGenerator(CollisionManager* colManager);

	void rootGenerator(CollisionManager* colManager, float x);

	void lanternGenerator(CollisionManager* colManager, Entity* tree_, float x,float y);

	bool getAble();
	void setAble(bool a);
private:
	Entity* player;
	Entity* tree_;
	bool isAble;

};

