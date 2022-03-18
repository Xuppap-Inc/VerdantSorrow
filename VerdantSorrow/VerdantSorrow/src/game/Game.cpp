#include <iostream>

#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

#include "Game.h"


Game::Game() : mngr_(nullptr)
{
}

Game::~Game()
{
	delete mngr_;
}

//Inicializa el juego y llama a las escenas
void Game::start() {
	int n = -1;

	std::cout << "0. Escena Hub" << std::endl << "1. Escena Rana" << std::endl << "2. Escena Arbol" << std::endl << "3. Escena Final" << std::endl;
	std::cin >> n;

	mngr_ = new ecs::Manager();

	SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	mngr_->changeScene(n);

	//Aqui haces el bucle
	//SceneManager->run()
}

void Game::update()
{
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

	SDL_Quit();
}
