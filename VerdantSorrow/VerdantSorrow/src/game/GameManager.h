#pragma once

#include <array>

#include "Container.h"

class GameManager
{
public:

	enum GameState {
		NEWGAME = 0, // just before starting a new game
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER // game over
	};

	GameManager() : state_(NEWGAME){}
	virtual ~GameManager() {}

	inline GameState getState() {
		return state_;
	}

	inline void setState(GameState state) {
		state_ = state;
	}


private:	
	GameState state_;
};

