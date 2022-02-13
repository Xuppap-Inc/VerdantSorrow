#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
	for (auto c : colliders) delete c;
}

std::vector<RectangleCollider*> CollisionManager::getCollisions(RectangleCollider* c)
{
	std::vector<RectangleCollider*> cols;

	for (auto col2 : colliders) {
	
		if (c != col2) {
		
			if (collision(c->getCollider(), col2->getCollider())) cols.push_back(col2);
		}
	}

	return cols;
}

bool CollisionManager::collision(SDL_Rect r1, SDL_Rect r2)
{
	if (SDL_HasIntersection(&r1, &r2) == SDL_TRUE) return true;

	return false;
}

bool CollisionManager::hasCollisions(RectangleCollider* c)
{
	return !getCollisions(c).empty();
}

void CollisionManager::addCollider()
{
}


