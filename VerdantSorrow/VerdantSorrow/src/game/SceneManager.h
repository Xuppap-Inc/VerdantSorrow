#pragma once
#include "../utils/Singleton.h"
class SceneManager :public Singleton<SceneManager> {

	friend Singleton<SceneManager>;
public:
	enum scenes { Hub, Frog, Tree, Eye };
	SceneManager();
	~SceneManager();
	scenes getScene() { return actScene; }
	void changeScene(scenes s_);
private:
	scenes actScene;

};

