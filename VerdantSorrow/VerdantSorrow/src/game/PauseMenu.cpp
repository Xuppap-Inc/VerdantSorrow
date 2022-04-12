#include "PauseMenu.h"
#include "../sdlutils/InputHandler.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../components/Transform.h"




PauseMenu::PauseMenu() :MenuScene(), controllerIndex_(-1), delay_(250), lastUpdate_(0), musicaTest_(nullptr), currentVolume_(180), changeSc_(false),varVolume_(128 / 8)
{
}

void PauseMenu::init()
{
	Scene::init();
	generateAllButtons();
	createImages((sdlutils().width() / 2) - 350, sdlutils().height() / 2 - (80 * 2), 80, 80, imagesNames_[0]);
	musicaTest_ = &sdlutils().musics().at("musica_rana_fase2");
	musicaTest_->play();
	currentVolume_ = musicaTest_->setMusicVolume(128);
}

void PauseMenu::onButtonClicked(int index)
{
	changeSc_ = true;

	switch (index)
	{
	case 0:
		controlVolume(false);
		break;
	case 1:
		controlVolume(true);
		break;
	case 2:
		break;
	case 3:
		sC().changeScene(SceneManager::Menu_);
		break;
	case 4:
		SDL_Quit();
		break;

	}
}

void PauseMenu::generateAllButtons()
{
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int spacingX = 250; int buttonW = 200, buttonH = 80, iniX = 350, smallButtonWH = 80;
	for (int i = 0; i < 2; ++i)
	{
		createButton(sdlutils().width() / 2 + (i * spacingX), sdlutils().height() / 2 - (smallButtonWH*2), smallButtonWH, smallButtonWH, buttonNames_[i]);
	}
	//Bucle que dibuja la fila de botones
	int j = 0;
	for (int i = 2; i < buttonNames_.size(); ++i)
	{
		createButton((sdlutils().width() / 2) - iniX + (j * spacingX), sdlutils().height() / 2, buttonW, buttonH, buttonNames_[i]);
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
					if (controllerIndex_ == -1 || controllerIndex_ == 0) changeButton(1);
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
		if (controllerIndex_ != -1 && controllerIndex_ < buttonNames_.size())
		{
			if (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A)) onButtonClicked(controllerIndex_);

			selectButton(controllerIndex_);
		}
	}
}
void PauseMenu::changeButton(int moves) //Controla la lógica entre el cambio de botones seleccionados con el mando
{
	//Controller index guarda el indice del boton sobre el que se encuentra el raton

	if (controllerIndex_ < buttonNames_.size() || controllerIndex_ == -1)
	{
		controllerIndex_ += moves; //Suma un determinado numero de "movimientos" necesarios para llegar al boton deseado
	}
	else controllerIndex_ = -1;

	lastUpdate_ = sdlutils().currRealTime(); //Para controlar el delay entre cambio de botones con mando
}

void PauseMenu::controlVolume(bool turnUp)
{
	int newVolume = 0;
	if (turnUp) newVolume = currentVolume_ + varVolume_; 
	else newVolume = currentVolume_ - varVolume_; 

	if (newVolume <= 128 && newVolume >= 0) currentVolume_ = newVolume;

	musicaTest_->setMusicVolume(currentVolume_);

}
void PauseMenu::createImages(float x, float y, float w, float h, std::string image)
{
	auto newImage = mngr_->addEntity();
	auto tr = newImage->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newImage->addComponent<Image>(&sdlutils().images().at(image));

}
