#pragma once
#include <vector>
#include "CollisionManager.h"
#include "Scene.h"
class Hub :public Scene
{
public:
	Hub() :Scene() {}
	~Hub() {}
	void init();

	virtual void update () override;
	void playerGenerator(CollisionManager* colManager, Entity* player_);
	void EntryGenerator(CollisionManager* colManager);
	void NPCGenerator(CollisionManager* colManager, Entity* dialogBox_);
	void dialogBoxGenerator(Entity* dialogBox);

};