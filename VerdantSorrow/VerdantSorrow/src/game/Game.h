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
	
	std::vector<Container*> objs_;
};

