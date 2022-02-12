#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "Container.h"
#include "GameManager.h"

Game::Game()
{
}

Game::~Game()
{
	for (Container* o : objs_) {
		delete o;
	}
}

void Game::init()
{
	SDLUtils::init("Verdant Sorrow", 800, 600, "resources/config/resources.json");
}

void Game::start()
{
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// handle input
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		for (auto& o : objs_) {
			o->handleInput();
		}

		// update
		for (auto& o : objs_) {
			o->update();
		}

		sdlutils().clearRenderer();

		// render
		for (auto& o : objs_) {
			o->render();
		}

		sdlutils().presentRenderer();
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}
