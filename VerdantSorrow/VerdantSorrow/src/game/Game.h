#pragma once

#include <vector>
namespace ecs {
	class Manager;
}

class Game
{
public: 
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	ecs::Manager* mngr_;
};

