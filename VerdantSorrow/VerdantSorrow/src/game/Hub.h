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

	void entryGenerator(CollisionManager* colManager);
	void playerGenerator(CollisionManager* colManager, Entity* player_);

private:
	ecs::Manager* mngr_;
};