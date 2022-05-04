#include "PauseMenu.h"
#include "../sdlutils/InputHandler.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../components/Transform.h"
#include "Hub.h"
#include "FrogScene.h"
#include "../components/boss/frog_boss/FrogAttackManager.h"
#include "TreeScene.h"
#include "../components/boss/tree_boss/TreeAttackManager.h"
#include "FinalBossScene.h"
#include "../components/boss/finalBoss/FinalBossMovement.h"
#include "MenuScene.h"
#include "EscapeScene.h"
#include "Game.h"




PauseMenu::PauseMenu() :BaseMenu(), controllerIndex_(-1), delay_(250), lastUpdate_(0), musicaTest_(nullptr), currentVolume_(180), varVolume_(128 / 8)
{
}

void PauseMenu::init()
{
	changeSc_ = false;
	Scene::init();
	background();
	isChangingScene(changeSc_);
	generateAllButtons();
	createImages((sdlutils().width() / 2) - 350, sdlutils().height() / 2 - (80 * 2), 80, 80, imagesNames_[0]);
}

void PauseMenu::update()
{
	handleInput(buttonPositions_,delay_,lastUpdate_,controllerIndex_,buttonNames_, buttonPoperties_, 3, 1); //Metodo para control de input 
	if (!changeSc_) {
		
		mngr_->update();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		string texto(std::to_string(*sC().getHubScene()->getMusicVolume() * 100 / 128));
		texto += " %";
		createText(sdlutils().width(), sdlutils().height() - 680, texto);
		sdlutils().presentRenderer();
	}
}

void PauseMenu::onButtonClicked(int index)
{
	
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
		changeSc_ = true;
		sC().activatePause();
		break;
	case 3:
		changeSc_ = true;
		sC().changePauseState(false);
		sC().changeScene(SceneManager::Menu_);
		break;
	case 4:
		Game::instance()->setExit(true);
		break;
	}
}

void PauseMenu::generateAllButtons()
{
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int spacingX = 250; int buttonW = 200, buttonH = 80, iniX = 350, smallButtonWH = 80;
	for (int i = 0; i < 2; ++i)
	{
		createButton(sdlutils().width() / 2 + (i * (spacingX + 270)) - 300, sdlutils().height() / 2 - (smallButtonWH*2),
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
	Music* musica = new Music();
	SoundEffect* soundEffect;
	int* volMusicaHub = nullptr; int* volMusicaFrog = nullptr; int* volMusicaTree = nullptr; int* volMusicaEye = nullptr;
	int* volMenu = nullptr; int* volEscapeScene = nullptr;
	volMusicaHub = sC().getHubScene()->getMusicVolume();
	volMenu = sC().getMenuScene()->getMusicVolume();
	volEscapeScene = sC().getEscapeScene()->getMusicVolume();
	volMusicaFrog = sC().getFrogScene()->getAttackManager()->getMusicVolume();
	volMusicaTree = sC().getTreeScene()->getTreeAttackManager()->getMusicVolume();
	volMusicaEye = sC().getEyeScene()->getFinalBossManager()->getMusicVolume();
	bool isFrog = false, isTree = false, isEye = false;
	FrogAttackManager* attackManagerFrog;
	TreeAttackManager* attackManagerTree;
	switch (sC().getPreviousScene())
	{
	case SceneManager::scenes::Hub_:
		musica = sC().getHubScene()->getMusic();
		break;
	case SceneManager::scenes::Frog_:
		isFrog = true;
		attackManagerFrog = sC().getFrogScene()->getAttackManager();
		if (attackManagerFrog->isSecondPhase()) { musica = attackManagerFrog->getMusicSecondPhase(); }
		else { soundEffect = attackManagerFrog->getMusicFirtPhase(); }
		break;
	case SceneManager::scenes::Tree_:
		isTree = true;
		attackManagerTree = sC().getTreeScene()->getTreeAttackManager();
		if (attackManagerTree->isSecondPhase()) { musica = attackManagerTree->getMusicSecondPhase(); }
		else { soundEffect = attackManagerTree->getMusicFirtPhase(); }
		break;
	case SceneManager::scenes::Eye_:
		isEye = true;
		musica = sC().getEyeScene()->getFinalBossManager()->getMusic();
		break;
	case SceneManager::scenes::Menu_:
		musica = sC().getMenuScene()->getMusic();
		break;
	case SceneManager::scenes::EscapeScene_:
		musica = sC().getEscapeScene()->getMusic();
		break;
	}



	int newVolume = 0;
	if (turnUp) newVolume = *volMusicaHub + varVolume_;
	else newVolume = *volMusicaHub - varVolume_;

	if (newVolume > 128) newVolume = 128;
	else if (newVolume < 0) newVolume = 0;

	if (newVolume <= 128 && newVolume >= 0) {
		*volMusicaHub = newVolume;
		*volMenu = newVolume;
		*volEscapeScene = newVolume;
		if (isFrog) *volMusicaFrog = newVolume;
		if (isTree) *volMusicaTree = newVolume;
		if (isEye) *volMusicaEye = newVolume;
	}
	if(musica->getMusic() != NULL) musica->setMusicVolume(*volMusicaHub);
	else soundEffect->setChannelVolume(*volMusicaHub, 0);
}
void PauseMenu::createImages(float x, float y, float w, float h, std::string image)
{
	auto newImage = mngr_->addEntity();
	auto tr = newImage->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newImage->addComponent<Image>(&sdlutils().imagesHub().at(image));
}

void PauseMenu::createText(float x, float y, std::string message)
{
	Texture text(sdlutils().renderer(), message,
		sdlutils().fontsHub().at("ARIAL48"), build_sdlcolor(0xFFFFFFFFF));

	SDL_Rect rect = build_sdlrect(
		(x - text.width()) / 2.0f, y, text.width(), text.height());

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	rect.x *= sW;
	rect.w *= sW;
	rect.y *= sH;
	rect.h *= sH;

	text.render(rect);
}

void PauseMenu::background()
{
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondoSet"));
	backgr_->addToGroup(ecs::_BACKGROUND_1_GRP);
}
