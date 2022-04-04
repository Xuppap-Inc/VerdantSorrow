#include "PauseMenu.h"
#include "../sdlutils/InputHandler.h"


PauseMenu::PauseMenu()
{
}

void PauseMenu::init()
{
	Scene::init();
	generateAllButtons();
	musicaTest_ = &sdlutils().musics().at("musica_rana_fase2");
	musicaTest_->play();
	currentVolume_=musicaTest_->setMusicVolume(128);
}

void PauseMenu::onButtonClicked(int index)
{
	switch (index)
	{
	case 0: 
		std::cout << "Has pulsado el boton de volumen" << std::endl;
		controlVolume();
		break;
	case 1:
		std::cout << "Has pulsado el boton de volver al juego" << std::endl;
		break;
	case 2:
		std::cout << "Has pulsado el boton de volver al menu" << std::endl;
		break;
	case 3:
		std::cout << "Has pulsado el boton de salir" << std::endl;
		SDL_Quit();
		break;

	}
}

void PauseMenu::generateAllButtons()
{
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int spacingX = 250; int buttonW = 200, buttonH = 80, iniX = 350, smallButtonWH = 80;

	createButton((sdlutils().width() / 2) - iniX, sdlutils().height() / 2 - smallButtonWH, smallButtonWH, smallButtonWH, buttonNames[0]);
	//Bucle que dibuja la fila de botones
	int j = 0;
	for (int i = 1; i < buttonNames.size(); ++i)
	{
		createButton((sdlutils().width() / 2) - iniX + (j * spacingX), sdlutils().height() / 2, buttonW, buttonH, buttonNames[i]);
		++j;
	}
}

void PauseMenu::handleControllerInput()
{
	auto& ihdlr = ih();
	if (ihdlr.controllerConnected()) //Input con el mando
	{
		if (ihdlr.isAxisMotionEvent())
		{
			if (delay_ + lastUpdate_ < sdlutils().currRealTime())
			{
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 0.9) //Movimiento hacia abajo
				{
					if(controllerIndex_==-1 || controllerIndex_ == 0) changeButton(1);
				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 0.9) //Movimieto hacia la derecha
				{
					changeButton(1); //Suma tres posiciones (el menu es simetrico y hay tres botones por columna)

				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -0.9) //Movimiento hacia la izquierda
				{
					changeButton(-1);
				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -0.9) //Movimiento hacia arriba
				{
					if (controllerIndex_ == 1)changeButton(-1);
				}

			}
		}
		if (controllerIndex_ != -1 && controllerIndex_ < buttonNames.size())
		{
			if (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A)) onButtonClicked(controllerIndex_);

			selectButton(controllerIndex_);
		}
	}
}
void PauseMenu::changeButton(int moves) //Controla la lógica entre el cambio de botones seleccionados con el mando
{
	//Controller index guarda el indice del boton sobre el que se encuentra el raton

	if (controllerIndex_ < buttonNames.size() || controllerIndex_ == -1)
	{
		controllerIndex_ += moves; //Suma un determinado numero de "movimientos" necesarios para llegar al boton deseado
	}
	else controllerIndex_ = -1;

	lastUpdate_ = sdlutils().currRealTime(); //Para controlar el delay entre cambio de botones con mando
}

void PauseMenu::controlVolume()
{
	//int newVolume =currentVolume_-(currentVolume_ * 30) / 100;
	int newVolume =currentVolume_-(128/8); //Habria que darle 8 veces paa que se silencie del todo
	if(newVolume>=0) currentVolume_ = newVolume;
	musicaTest_->setMusicVolume(currentVolume_);
}
