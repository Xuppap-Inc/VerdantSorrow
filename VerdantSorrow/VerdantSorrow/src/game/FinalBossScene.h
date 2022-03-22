	#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"


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
	 
	void playerGenerator(CollisionManager* colManager, Entity* player_);

	bool getAble();
	void setAble(bool a);
private:
	bool isAble;
	Entity* player;
};

