#include "SceneManager.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"
#include "MenuScene.h"
#include "../sdlutils/SDLUtils.h"


SceneManager::SceneManager() : actScene(Hub_)
{
	h_ = new Hub(); sceneList.push_back(h_);
	f_ = new FrogScene(); sceneList.push_back(f_);
	t_ = new TreeScene(); sceneList.push_back(t_);
	fin_ = new FinalBossScene(); sceneList.push_back(fin_);
	tut_ = new TutorialScene(); sceneList.push_back(tut_);
	menu_ = new MenuScene(); sceneList.push_back(menu_);
}


SceneManager::~SceneManager()
{
	for (auto s : sceneList) {
		delete s;
	}
	sceneList.empty();
}

void SceneManager::update()
{
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
	default:
		break;
	}
}

void SceneManager::init()
{
	auto& sdlUtils_ = sdlutils();
	sdlUtils_.freeMemory();
	switch (actScene)
	{
	case SceneManager::Hub_:
		sdlUtils_.loadReasources("resources/config/hub.json");
		h_->init();
		break;
	case SceneManager::Frog_:
		sdlUtils_.loadReasources("resources/config/frog.json");
		//desactiva la escena para que no se pueda volver a acceder 
		//f_->setAble(false);
		f_->init();
		break;
	case SceneManager::Tree_:
		sdlUtils_.loadReasources("resources/config/treeScene.json");
		//t_->setAble(false);
		t_->init();
		break;
	case SceneManager::Eye_:
		sdlUtils_.loadReasources("resources/config/finalBoss.json");
		//fin_->setAble(false);
		fin_->init();
		break;
	case SceneManager::Tutorial_:
		sdlUtils_.loadReasources("resources/config/tutorial.json");
		tut_->init();
		break;
	case SceneManager::Menu_:
		sdlUtils_.loadReasources("resources/config/menu.json");
		menu_->init();
		break;
	default:
		break;
	}
}

void SceneManager::render()
{
}




void SceneManager::changeScene(scenes s_)
{
	actScene = s_;
	init();
}

void SceneManager::decideScene()
{

	if (actScene != Hub_) {
		actScene = Hub_;
		h_->changeScene_(false);
	}
	else if (f_->getAble()) {
		actScene = Frog_;
		//activa la siguiente escena
		//f_->setAble(false);
		//t_->setAble(true);
	}
	else if (t_->getAble()) {
		actScene = Tree_;
		//fin_->setAble(true);
	}
	else if (fin_->getAble()){
		actScene = Eye_;
	}
	init();
}