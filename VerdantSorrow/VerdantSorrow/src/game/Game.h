#pragma once
#include "GameManager.h"
#include <vector>
#include "box2d.h"

class Container;
class GameManager;

class Game
{
public: 
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	Container* player;
	Container* player2;
	std::vector<Container*> objs_;
	b2World mundo;
	b2Body* body;
	b2Body* body2;
};

