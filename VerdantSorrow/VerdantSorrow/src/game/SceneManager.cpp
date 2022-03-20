#include "SceneManager.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"


SceneManager::SceneManager() : actScene(Hub_)
{
	h_ = new Hub();
	f_ = new FrogScene();
	t_ = new TreeScene();
	fin_ = new FinalBossScene();
	tut_ = new TutorialScene();
}

SceneManager::~SceneManager()
{
	delete h_; delete f_; delete t_; delete fin_; delete tut_;
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
	default:
		break;
	}
}

void SceneManager::init()
{
	switch (actScene)
	{
	case SceneManager::Hub_:
		h_->init();
		break;
	case SceneManager::Frog_:
		f_->init();
		break;
	case SceneManager::Tree_:
		t_->init();
		break;
	case SceneManager::Eye_:
		fin_->init();
		break;
	case SceneManager::Tutorial_:
		tut_->init();
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
	//init();
}
