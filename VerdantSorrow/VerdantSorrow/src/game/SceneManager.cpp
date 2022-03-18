#include "SceneManager.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"
SceneManager::SceneManager(): actScene(Hub)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::update()
{
}

void SceneManager::render()
{
}




void SceneManager::changeScene(scenes s_)
{	
	actScene = s_;
}
