#include "Settings.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
Settings::Settings()
{

}
void Settings::init()
{
	changeSc_ = false;
	Scene::init();
	isChangingScene(changeSc_);
	generateAllButtons();
}

void Settings::update()
{
	handleInput(buttonPositions_, delay_, lastUpdate_, controllerIdex_, buttonNames_, buttonPoperties_);
	if (!changeSc_)
	{
		mngr_->update();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}

}

void Settings::onButtonClicked(int index)
{
	changeSc_ = true;
	isChangingScene(changeSc_);

	switch (index)
	{
	case 0: //Boton salida al menu principal
		if (sC().getPreviousScene() == SceneManager::Menu_) {
			sC().changeScene(SceneManager::Menu_);
		}
		break;
	}
}

void Settings::generateAllButtons()
{
	
	////Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	//int spacingX = 250; int buttonW = 200, buttonH = 80, iniX = 350, smallButtonWH = 80;
	//for (int i = 0; i < 2; ++i)
	//{
	//	createButton(sdlutils().width() / 2 + (i * spacingX), sdlutils().height() / 2 - (smallButtonWH * 2),
	//		smallButtonWH, smallButtonWH, buttonNames_[i], buttonPositions_, buttonPoperties_);
	//}
	////Bucle que dibuja la fila de botones
	//int j = 0;
	//for (int i = 2; i < buttonNames_.size(); ++i)
	//{
	//	createButton((sdlutils().width() / 2) - iniX + (j * spacingX), sdlutils().height() / 2, buttonW, buttonH,
	//		buttonNames_[i], buttonPositions_, buttonPoperties_);
	//	++j;
	//}
	



	int buttonWH = 50;
	createButton(0, sdlutils().height() - buttonWH, buttonWH, buttonWH, buttonNames_[0], buttonPositions_, buttonPoperties_);
}

void Settings::background()
{
}


