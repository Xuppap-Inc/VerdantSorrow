#include "PauseMenu.h"
#include "../sdlutils/InputHandler.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../components/Transform.h"




PauseMenu::PauseMenu() :BaseMenu(), controllerIndex_(-1), delay_(250), lastUpdate_(0), musicaTest_(nullptr), currentVolume_(180), changeSc_(false),varVolume_(128 / 8)
{
}

void PauseMenu::init()
{
	Scene::init();
	isChangingScene(changeSc_);
	generateAllButtons();
	createImages((sdlutils().width() / 2) - 350, sdlutils().height() / 2 - (80 * 2), 80, 80, imagesNames_[0]);
	musicaTest_ = &sdlutils().musics().at("musica_rana_fase2");
	musicaTest_->play();
	currentVolume_ = musicaTest_->setMusicVolume(128);
}

void PauseMenu::update()
{
	handleInput(buttonPositions_,delay_,lastUpdate_,controllerIndex_,buttonNames_, buttonPoperties_); //Metodo para control de input 
	if (!changeSc_) {
		mngr_->update();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}
}

void PauseMenu::onButtonClicked(int index)
{
	changeSc_ = true;
	isChangingScene(changeSc_);
	switch (index)
	{
	case 0:
		controlVolume(false);
		break;
	case 1:
		controlVolume(true);
		break;
	case 2:
		sC().activatePause();
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
		createButton(sdlutils().width() / 2 + (i * spacingX), sdlutils().height() / 2 - (smallButtonWH*2),
			smallButtonWH, smallButtonWH, buttonNames_[i],buttonPositions_, buttonPoperties_);
	}
	//Bucle que dibuja la fila de botones
	int j = 0;
	for (int i = 2; i < buttonNames_.size(); ++i)
	{
		createButton((sdlutils().width() / 2) - iniX + (j * spacingX), sdlutils().height() / 2, buttonW, buttonH, 
			buttonNames_[i],buttonPositions_, buttonPoperties_);
		++j;
	}
}

void PauseMenu::controlVolume(bool turnUp)
{
	int newVolume = 0;
	if (turnUp) newVolume = currentVolume_ + varVolume_; 
	else newVolume = currentVolume_ - varVolume_; 

	if (newVolume > 128) newVolume = 128;
	else if (newVolume < 0) newVolume = 0;

	if (newVolume <= 128 && newVolume >= 0) currentVolume_ = newVolume;


	musicaTest_->setMusicVolume(currentVolume_);

}
void PauseMenu::createImages(float x, float y, float w, float h, std::string image)
{
	auto newImage = mngr_->addEntity();
	auto tr = newImage->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newImage->addComponent<Image>(&sdlutils().images().at(image));

}
