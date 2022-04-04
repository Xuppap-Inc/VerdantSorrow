	#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"

class CollisionChecker;
class FinalBossScene: public Scene
{
public: 
	FinalBossScene() :Scene() {}
	~FinalBossScene() {};
	void init();

	void update()override;
	void waveSpawnerGenerator(CollisionManager*& colManager);
	//start lo hace scene directamente
	void background();
	void finalBossGenerator(CollisionManager* colManager, Entity* player_);

	bool getAble();
	void setAble(bool a);
	void createLights();
private:
	bool isAble;
	Entity* player;
	Entity* FinalBossFace;
	CollisionChecker* colCheck_;
};

