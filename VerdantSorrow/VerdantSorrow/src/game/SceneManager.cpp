#include "SceneManager.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"


SceneManager::SceneManager() : actScene(Hub_)
{
	h_ = new Hub(); sceneList.push_back(h_);
	f_ = new FrogScene(); sceneList.push_back(f_);
	t_ = new TreeScene(); sceneList.push_back(t_);
	fin_ = new FinalBossScene(); sceneList.push_back(fin_);
	tut_ = new TutorialScene(); sceneList.push_back(tut_);
}

SceneManager::~SceneManager()
{
	delete h_; delete f_; delete t_; delete fin_; delete tut_;
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
