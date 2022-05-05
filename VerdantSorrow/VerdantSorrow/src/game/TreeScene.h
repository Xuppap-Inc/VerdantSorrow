#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"

class CollisionChecker;
class TreeAttackManager;
class TreeScene :public Scene
{
public:
	TreeScene() :Scene(), isAble(false) {}
	~TreeScene() {}
	void init();

	void update()override;
	void treeGenerator(CollisionManager* colManager);

	void lanternGenerator(CollisionManager* colManager, Entity* tree_, float x,float y);

	void deactivateBoss() override;

	bool getAble();
	void setAble(bool a);

	TreeAttackManager* getTreeAttackManager() { return treeMngr_; };
private:
	Entity* player;
	Entity* tree_;
	bool isAble;
	CollisionChecker* colCheck_;

	TreeAttackManager* treeMngr_;
};

