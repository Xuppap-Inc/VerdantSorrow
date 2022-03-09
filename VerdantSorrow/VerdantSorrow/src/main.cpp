// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>

#include "ecs/Manager.h"
#include "sdlutils/SDLUtils.h"
void start() {
	int n = -1;

	std::cout << "0. Escena Hub" << std::endl << "1. Escena Rana" << std::endl << "2. Escena Arbol" << "3. Escena Final" << std::endl << std::endl;
	std::cin >> n;
	ecs::Manager* mngr_ = new ecs::Manager();
	SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	mngr_->changeScene(n);
}
int main(int ac, char **av) {

	try {
		start();
	} catch (const std::string &e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	} catch (const char *e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	} catch (const std::exception &e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}

