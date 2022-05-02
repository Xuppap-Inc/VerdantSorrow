	#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"

class CollisionChecker;
class FinalBossMovement;
class HandsManager;
class FinalBossScene: public Scene
{
public: 
	FinalBossScene() :Scene(), isAble(false) {}
	~FinalBossScene() {};
	void init();

	void update()override;
	void waveSpawnerGenerator(CollisionManager*& colManager);
	//start lo hace scene directamente
	void background();
	void finalBossGenerator(CollisionManager* colManager, Entity* player_);

	void deactivateBoss() override;

	bool getAble();
	void setAble(bool a);
	void createLights();
	FinalBossMovement* getFinalBossManager() { return movement_; };
private:
	bool isAble;
	Entity* player;
	Entity* FinalBossFace;
	CollisionChecker* colCheck_;

	HandsManager* handsMngr_;
	FinalBossMovement* movement_;
};

