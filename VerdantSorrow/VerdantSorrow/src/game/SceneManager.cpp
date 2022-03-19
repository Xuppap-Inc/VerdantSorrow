#include "SceneManager.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"


SceneManager::SceneManager(): actScene(Hub_)
{
	Hub* h_ = new Hub();
	FrogScene* f_ = new FrogScene();
	TreeScene* t_ = new TreeScene();
	FinalBossScene* fin_ = new FinalBossScene();
}

SceneManager::~SceneManager()
{
}

void SceneManager::update()
{
	switch (actScene)
	{
	case SceneManager::Hub_:

		break;
	case SceneManager::Frog_:
		break;
	case SceneManager::Tree_:
		break;
	case SceneManager::Eye_:
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
}
