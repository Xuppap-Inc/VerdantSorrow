#include "SceneManager.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"
#include "MenuScene.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "TutorialScene.h"
#include "ControlsScene.h"
#include "PauseMenu.h"
#include "EscapeScene.h"
#include "Settings.h"


SceneManager::SceneManager() : actScene(Hub_), frogEssenceObtained_(false), treeEssenceObtained_(false), eyeEssenceObtained_(false), hubAssetsChargeds_(false),
playerInBossFight(false),previousScene_(Hub_),isPauseActive_(false)
{
	h_ = new Hub(); sceneList.push_back(h_);
	f_ = new FrogScene(); sceneList.push_back(f_);
	t_ = new TreeScene(); sceneList.push_back(t_);
	fin_ = new FinalBossScene(); sceneList.push_back(fin_);
	tut_ = new TutorialScene(); sceneList.push_back(tut_);
	menu_ = new MenuScene(); sceneList.push_back(menu_);
	controls_ = new ControlsScene(); sceneList.push_back(controls_);
	pauseMenu_ = new PauseMenu(); sceneList.push_back(pauseMenu_);
	ecapesc_ = new EscapeScene(); sceneList.push_back(ecapesc_);
	settingsMenu_ = new Settings(); sceneList.push_back(settingsMenu_);

}

SceneManager::~SceneManager()
{
	for (auto s : sceneList) {
		delete s;
	}
	sceneList.clear();
}

void SceneManager::update()
{
	SDL_GameControllerButton pauseGamepadButton = SDL_CONTROLLER_BUTTON_START;
	SDL_Scancode pauseKeyboardButton = SDL_SCANCODE_ESCAPE;

	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent() || ihdlr.controllerDownEvent())
	{
		if (ihdlr.isKeyDown(pauseKeyboardButton)|| ihdlr.isControllerButtonDown(pauseGamepadButton))
		{
			if((previousScene_ != Menu_ && actScene != Menu_) || (previousScene_ == Menu_ && actScene == Hub_))
			{
				activatePause();
			}
		}
	}
	switch (actScene)
	{
	case SceneManager::Hub_:
		h_->update();
		break;
	case SceneManager::Frog_:
		f_->update();
		break;
	case SceneManager::Tree_:
		t_->update();
		break;
	case SceneManager::Eye_:
		fin_->update();
		break;
	case SceneManager::Tutorial_:
		tut_->update();
		break;
	case SceneManager::Menu_:
		menu_->update();
		break;
	case SceneManager::Controls_:
		controls_->update();
		break;
	case SceneManager::PauseMenu_:
		pauseMenu_->update();
		break;
	case SceneManager::Settings_:
		settingsMenu_->update();
		break;
	case SceneManager::EscapeScene_:
		ecapesc_->update();
		break;
	default:
		break;
	}
	
}

void SceneManager::init()
{
	auto& sdlUtils_ = sdlutils();

	if (!hubAssetsChargeds_) {
		sdlUtils_.loadReasourcesHub("resources/config/hub.json");
		hubAssetsChargeds_ = true;
	}
	

	if(!playerInBossFight) 
		sdlUtils_.freeMemory();
	switch (actScene)
	{
	case SceneManager::Hub_:
		h_->init();
		break;
	case SceneManager::Frog_:
		if (!playerInBossFight) {
			sdlUtils_.loadReasources("resources/config/frog.json");
			playerInBossFight = true;
		}
		f_->init();
		break;
	case SceneManager::Tree_:
		if (!playerInBossFight) {
			sdlUtils_.loadReasources("resources/config/treeScene.json");
			playerInBossFight = true;
		}
		t_->init();
		break;
	case SceneManager::Eye_:
		if (!playerInBossFight) {
			sdlUtils_.loadReasources("resources/config/finalBoss.json");
			playerInBossFight = true;
		}	
		fin_->init();
		break;
	case SceneManager::Tutorial_:
		if (!playerInBossFight) {
			playerInBossFight = true;
			sdlUtils_.loadReasources("resources/config/tutorial.json");
		}
		tut_->init();
		break;
	case SceneManager::Menu_:
		playerInBossFight = false;
		menu_->init();
		break;
	case SceneManager::Controls_:
		playerInBossFight = false;
		controls_->init();
		break;
	case SceneManager::PauseMenu_:
		pauseMenu_->init();
		break;
	case SceneManager::Settings_:
		settingsMenu_->init();
		break;
	case SceneManager::EscapeScene_:
		if (!playerInBossFight) {
			playerInBossFight = true;
			sdlUtils_.loadReasources("resources/config/escapeScene.json");
		}
		ecapesc_->init();
		break;
	default:
		break;
	}
	FrogSceneState(false);
	TreeSceneState(false);
	EyeSceneState(false);
}

void SceneManager::render()
{
}

void SceneManager::changeScene(scenes s_)
{
	previousScene_ = getScene();
	actScene = s_;
	init();
}

void SceneManager::changeFrogEssenceState(bool active)
{
	frogEssenceObtained_ = active;
}

void SceneManager::changeTreeEssenceState(bool active)
{
	treeEssenceObtained_ = active;
}

void SceneManager::changeEyeEssenceState(bool active)
{
	eyeEssenceObtained_ = active;
}

void SceneManager::FrogSceneState(bool active) {
	f_->setAble(active);
}

void SceneManager::TreeSceneState(bool active) {
	t_->setAble(active);
}

void SceneManager::EyeSceneState(bool active) {
	fin_->setAble(active);
}

void SceneManager::changeStatePlayerInBoss(bool active)
{
	playerInBossFight = active;
}

void SceneManager::activatePause()
{
	if (isPauseActive_) //Si se le ha dado a esc y la pausa estaba ya activa
	{
		//changeScene(previousScene_); //cambia a la escena en la que estabas antes de la pausa
		actScene = previousScene_;
		pauseMenu_->changeScState(true);
	}
	else //si la pausa no estaba activa al darle a esc
	{
		pauseMenu_->changeScState(false);
		previousScene_ = getScene(); //guarda la escena en la que estas
		changeScene(PauseMenu_); //cambia a la pausa
	}

	isPauseActive_ =!isPauseActive_;
}

void SceneManager::decideScene()
{
	if (actScene != Hub_) {
		actScene = Hub_;
		h_->changeScene_(false);
	}
	else if (f_->getAble()) {
		previousScene_ = Hub_;
		actScene = Frog_;
		t_->setAble(true);
	}
	else if (t_->getAble()) {
		previousScene_ = Hub_;
		actScene = Tree_;
		fin_->setAble(true);
	}
	else if (fin_->getAble()) {
		previousScene_ = Hub_;
		actScene = Eye_;
	}
	init();
}

void SceneManager::setFirstSceneAble()
{
	f_->setAble(true);
}
