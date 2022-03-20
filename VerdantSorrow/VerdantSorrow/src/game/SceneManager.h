#pragma once
#include "../utils/Singleton.h"
#include "Scene.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"
#include "TutorialScene.h"

#include <vector>
class SceneManager :public Singleton<SceneManager> {

	friend Singleton<SceneManager>;
public:
	enum scenes { Hub_, Frog_, Tree_, Eye_, Tutorial_ };
	SceneManager();
	~SceneManager();
	void update();
	//inicializa la escena en la que está
	void init();
	void render();
	scenes getScene() { return actScene; }
	void changeScene(scenes s_=Hub_);
private:
	scenes actScene;
	std::vector <Scene*> sceneList;
	Hub* h_;
	FrogScene* f_;
	TreeScene* t_;
	FinalBossScene* fin_;
	TutorialScene* tut_;
};

