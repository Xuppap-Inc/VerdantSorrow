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

	void playerGenerator(CollisionManager* colManager, Entity* player);

private:
	ecs::Manager* mngr_;
};