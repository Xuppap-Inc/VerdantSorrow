#pragma once
#include <vector>
#include "CollisionManager.h"
#include "Scene.h"
class Hub :public Scene
{
public:
	Hub();
	~Hub();
	void init();

	virtual void update () override;
	void playerGenerator(CollisionManager* colManager, Entity* player_);
	void EntryGenerator(CollisionManager* colManager);
	void NPCGenerator(CollisionManager* colManager, Entity* dialogBox_);
	void dialogBoxGenerator(Entity* dialogBox);
	bool getAble();
	void setAble(bool a);
	void changeScene_();
	void checkCollissions();
private:
	CollisionManager* colManager;
	bool isAble;
	bool changeSc;
	Entity* player;
};