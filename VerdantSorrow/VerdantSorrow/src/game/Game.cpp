#include <iostream>

#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

#include "Game.h"

//Inicializa el juego y llama a las escenas
void Game::start() {
	int n = -1;

	std::cout << "0. Escena Hub" << std::endl << "1. Escena Rana" << std::endl << "2. Escena Arbol" << std::endl << "3. Escena Final" << std::endl;
	std::cin >> n;

	ecs::Manager* mngr_ = new ecs::Manager();

	SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	mngr_->changeScene(n);

	//Aqui haces el bucle
	//SceneManager->run()
}