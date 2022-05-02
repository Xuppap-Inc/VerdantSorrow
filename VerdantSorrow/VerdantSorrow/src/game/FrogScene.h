#pragma once

#include <vector>
#include "CollisionManager.h"
#include "Scene.h"

class CollisionChecker;
class FrogAttackManager;
class FrogScene: public Scene
{
public: 
	FrogScene() :Scene(), isAble(false) {};
	~FrogScene() {};
	void init();

	void update()override;
	void waveSpawerGenerator(CollisionManager*& colManager);
	void background();
	void frogGenerator(CollisionManager* colManager, Entity* player_);

	void deactivateBoss() override;

	bool getAble();
	void setAble(bool a);
	FrogAttackManager* getAttackManager() { return frogMngr_; };
private:
	bool isAble;
	Entity* player;
	Entity* Frog;
	CollisionChecker* colCheck_;

	FrogAttackManager* frogMngr_;

};

