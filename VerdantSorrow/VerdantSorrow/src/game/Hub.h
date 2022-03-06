#pragma once
#include <vector>
#include "CollisionManager.h"
namespace ecs {
	class Manager;
}
class Hub
{
public:
	Hub();
	virtual ~Hub();
	void init();
	void start();

	void playerGenerator(CollisionManager* colManager, Entity* player_);
	void EntryGenerator(CollisionManager* colManager);
	void NPCGenerator(CollisionManager* colManager, Entity* dialogBox_);
	void dialogBoxGenerator(Entity* dialogBox);

private:
	ecs::Manager* mngr_;
};