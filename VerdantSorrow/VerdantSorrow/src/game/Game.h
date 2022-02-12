#pragma once
#include "GameManager.h"
#include <vector>

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
	std::vector<Container*> objs_;
};

