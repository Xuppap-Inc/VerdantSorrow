#include "SceneManager.h"

SceneManager::SceneManager(): actScene(Hub)
{
}

SceneManager::~SceneManager()
{
}




void SceneManager::changeScene(scenes s_)
{
	actScene = s_;
}
