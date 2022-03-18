#pragma once
#include "../utils/Singleton.h"
class Game :public Singleton<Game>
{
	friend Singleton<Game>;
public:
	static void start();
};

