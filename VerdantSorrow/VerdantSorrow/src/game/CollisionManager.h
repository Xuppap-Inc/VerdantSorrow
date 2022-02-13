#pragma once
#include <vector>
#include "RectangleCollider.h"

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	std::vector<RectangleCollider*> getCollisions(RectangleCollider* c);

	bool collision(SDL_Rect r1, SDL_Rect r2);

	bool hasCollisions(RectangleCollider* c);

protected:
	std::vector<RectangleCollider*> colliders;
};

