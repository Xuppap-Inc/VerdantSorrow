#pragma once
#include "../utils/Singleton.h"
#include "Scene.h"
#include <stack>
class SceneManager :public Singleton<SceneManager> {

	friend Singleton<SceneManager>;
public:
	enum scenes { Hub_, Frog_, Tree_, Eye_ };
	SceneManager();
	~SceneManager();
	void update();
	void render();
	scenes getScene() { return actScene; }
	void changeScene(scenes s_);
private:
	scenes actScene;
	std::stack <Scene*> sceneList;
};

