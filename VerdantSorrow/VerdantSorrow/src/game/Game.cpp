#include <iostream>

#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "SceneManager.h"

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

	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");

	int n = -1;
	std::cout << "0.Cambio de escenas normal\n1.Elegir escena\n";
	std::cin >> n;
	SceneManager::scenes firstScene;
	if (n == 1) {
		std::cout << "0. Escena Hub\n1. Escena Rana\n2. Escena Arbol\n3. Escena Final\n4. Tutorial\n5. Menu\n6. Controles\n7.Pausa\n";
		std::cin >> n;
		switch (n)
		{
		case 0:
			firstScene = SceneManager::scenes::Hub_;
			break;
		case 1:
			firstScene = SceneManager::scenes::Frog_;
			break;
		case 2:
			firstScene = SceneManager::scenes::Tree_;
			break;
		case 3:
			firstScene = SceneManager::scenes::Eye_;
			break;
		case 4:
			firstScene = SceneManager::scenes::Tutorial_;
			break;
		case 5:
			firstScene = SceneManager::scenes::Menu_;
			break;
		case 6:
			firstScene = SceneManager::scenes::Controls_;
			break;
		case 7:
			firstScene = SceneManager::scenes::PauseMenu_;
			break;

		default:
			break;
		}
	}
	else {
		firstScene = SceneManager::scenes::Hub_;
		sC().setFirstSceneAble();
	}
	
	SDLUtils::init("Verdant Sorrow", 1280, 720);
	//Crea el game Manager
	//SDLUtils::init("Verdant Sorrow", 1280, 720, json);

	auto& scMngr_ = sC();
	
	scMngr_.changeScene(firstScene);
	//scMngr_.init();


	//mngr_->changeScene(n);
	//Aqui haces el bucle
	//SceneManager->run()
}

void Game::update()
{
	// a boolean to exit the loop
	bool exit = false;

	//Imput handler
	auto& ihdlr = ih();

	auto& scMngr_ = sC();
	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
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
