#include <iostream>

#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "SceneManager.h"
#include <fstream>
#include "Game.h"


Game::Game() : mngr_(nullptr), exit(false),state_(HUB), playerJustKilled(false)
{
}

Game::~Game()
{
	delete mngr_;
}

//Inicializa el juego y llama a las escenas
void Game::start() {

	exit = false;

	int w = 1280, h = 720;

	SceneManager::scenes firstScene;
	firstScene = SceneManager::scenes::Menu_;
	sC().setFirstSceneAble();
	
	SDLUtils::init("Verdant Sorrow", w, h);
	auto& scMngr_ = sC();
	scMngr_.changeScene(firstScene);
	update();
}

void Game::update()
{
	//Imput handler
	auto& ihdlr = ih();

	auto& scMngr_ = sC();
	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isQuitPressed()) 
		{
			exit = true;
			continue;
		}

		scMngr_.update();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
	SDL_Quit();
}

void Game::setExit(bool set)
{
	exit = set;
}
