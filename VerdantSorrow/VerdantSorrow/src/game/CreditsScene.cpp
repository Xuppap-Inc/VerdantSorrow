#include "CreditsScene.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../components/Transform.h"

CreditsScene::CreditsScene() : delay_(250), lastUpdate_(0), controllerIdex_(-1), musicVolume_(60)
{

}
void CreditsScene::init()
{
	changeSc_ = false;
	Scene::init();
	background();
	isChangingScene(changeSc_);
	generateAllButtons();

	//Musica
	musica_ = &sdlutils().musicsHub().at("music_credits");
	musica_->play();
	musica_->setMusicVolume(musicVolume_);
	musica_->resumeMusic();
}

void CreditsScene::update()
{
	handleInput(buttonPositions_, delay_, lastUpdate_, controllerIdex_, buttonNames_, buttonPoperties_, 1, 1);
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

void CreditsScene::onButtonClicked(int index)
{

	isChangingScene(changeSc_);
	musica_->pauseMusic();
	switch (index)
	{
	case 0: //Boton salida al menu principal
		if (sC().getPreviousScene() == SceneManager::Menu_) {
			sC().changeScene(SceneManager::Menu_);
			changeSc_ = true;
		}
		else {
			sC().changeScene(SceneManager::Hub_);
			changeSc_ = true;
		}
		break;
	}
}

void CreditsScene::generateAllButtons()
{
	int buttonWH = 50;
	createButton(0, sdlutils().height() - buttonWH, buttonWH, buttonWH, buttonNames_[0], buttonPositions_, buttonPoperties_);
}

void CreditsScene::background()
{
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondoSet"));
	backgr_->addToGroup(ecs::_BACKGROUND_1_GRP);
}

