#include "PauseMenu.h"

PauseMenu::PauseMenu()
{
}

void PauseMenu::init()
{
	Scene::init();
	generateAllButtons();
}

void PauseMenu::onButtonClicked(int index)
{
	switch (index)
	{
	case 0: //Boton resume
		std::cout << "Has pulsado el boton de volver al juego" << std::endl;
		break;
	case 1: //Boton menu
		std::cout << "Has pulsado el boton de volver al menu" << std::endl;

		break;
	case 2: //Boton quit
		std::cout << "Has pulsado el boton de salir" << std::endl;
		SDL_Quit();
		break;
	
	}
}

void PauseMenu::generateAllButtons()
{
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int spacingX = 250; int buttonW = 200, buttonH = 80, iniX=350; 

	//Bucle que dibuja la primera columna (izq) de botones
	for (int i = 0; i < buttonNames.size(); ++i)
	{
		createButton((sdlutils().width() / 2)-iniX +(i*spacingX), sdlutils().height()/2, buttonW, buttonH, buttonNames[i]);
	}
}
