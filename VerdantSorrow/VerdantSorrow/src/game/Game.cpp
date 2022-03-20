#include <iostream>

#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "SceneManager.h"

#include "Game.h"


Game::Game() : mngr_(nullptr),scMngr_(nullptr)
{
}

Game::~Game()
{
	delete mngr_;
	delete scMngr_;
}

//Inicializa el juego y llama a las escenas
void Game::start() {
	int n = -1;

	//Método temporal para poder acceder a todas las escenas
	std::cout << "0. Escena Hub" << std::endl << "1. Escena Rana" << std::endl << "2. Escena Arbol" 
		<< std::endl << "3. Escena Final" << std::endl << "4. Tutorial" << std::endl;
	std::cin >> n;

	//Crea el game Manager
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

	//Imput handler
	auto& ihdlr = ih();

	scMngr_ = new SceneManager();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		scMngr_->update();
		/*mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();*/

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
	SDL_Quit();
}
