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
	std::string json;
	std::cout << "0.Cambio de escenas normal\n1.Elegir escena\n";
	std::cin >> n;
	SceneManager::scenes firstScene;
	if (n == 1) {
		std::cout << "0. Escena Hub\n1. Escena Rana\n2. Escena Arbol\n3. Escena Final\n4. Tutorial\n5. Menu\n";
		std::cin >> n;
		switch (n)
		{
		case 0:
			firstScene = SceneManager::scenes::Hub_;
			json = "resources/config/hub.json";
			break;
		case 1:
			firstScene = SceneManager::scenes::Frog_;
			json = "resources/config/frog.json";
			break;
		case 2:
			firstScene = SceneManager::scenes::Tree_;
			json = "resources/config/groot.json";
			break;
		case 3:
			firstScene = SceneManager::scenes::Eye_;
			json = "resources/config/finalBoss.json";
			break;
		case 4:
			firstScene = SceneManager::scenes::Tutorial_;
			json = "resources/config/tutorial.json";
			break;
		case 5:
			firstScene = SceneManager::scenes::Menu_;
			json = "resources/config/menu.json";
			break;

		default:
			break;
		}
	}
	else {
		firstScene = SceneManager::scenes::Hub_;
		json = "resources/config/hub.json";
	}
	
	SDLUtils::init("Verdant Sorrow", 1280, 720, json);
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
