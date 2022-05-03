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

	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	exit = false;

	int w = 0, h = 0;

	int r = -1;
	std::cout << "Resolucion:\n0. 1080p\n1. 720p\n2. 480p\n3. 144p\n";
	
	while (r != 0 && r != 1 && r != 2 && r != 3) std::cin >> r;

	switch (r)
	{
	case 0: w = 1920;
		h = 1080;
		break;
	case 1: w = 1280;
		h = 720;
		break;
	case 2: w = 850;
		h = 480;
		break;
	case 3: w = 16 * 16;
		h = 9 * 16;
		break;
	default:
		break;
	}
	
	int n = -1;
	std::cout << "0.Cambio de escenas normal\n1.Elegir escena\n";
	std::cin >> n;
	SceneManager::scenes firstScene;
	if (n == 1) {
		std::cout << "0. Escena Hub\n1. Escena Rana\n2. Escena Arbol\n3. Escena Final\n4. Tutorial\n5. Menu\n6. Controles\n7. Pausa\n8. EscapeScene\n";
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
		case 8:
			firstScene = SceneManager::scenes::EscapeScene_;
			break;

		default:
			break;
		}
	}
	else {
		firstScene = SceneManager::scenes::Menu_;
		sC().setFirstSceneAble();
	}

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
