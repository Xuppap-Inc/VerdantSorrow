// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>

#include "game/FrogScene.h"
#include "game/TreeScene.h"
#include "game/Hub.h"
void start() {

	int n = -1;

	std::cout << "0. Escena Hub" << std::endl << "1. Escena Rana" << std::endl << "2. Escena Árbol" << std::endl;
	std::cin >> n;

	if (n == 0) {
	
		Hub h;
		h.init();
		h.start();
	}
	else if (n == 1) {

		FrogScene f;
		f.init();
		f.start();
	}
	else if (n == 2) {

		TreeScene t;
		t.init();
		t.start();
	}
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

