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
	int buttonWH = 50;
	createButton(0, sdlutils().height() - buttonWH, buttonWH, buttonWH, buttonNames_[0], buttonPositions_, buttonPoperties_);
}

void Settings::background()
{
}


