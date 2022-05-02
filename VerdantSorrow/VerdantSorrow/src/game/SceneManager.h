#pragma once
#include "../utils/Singleton.h"
class Scene;
class FrogScene;
class TreeScene;
class FinalBossScene;
class Hub;
class TutorialScene;
class MenuScene;
class ControlsScene;
class EscapeScene;
class PauseMenu;
#include <vector>
class SceneManager :public Singleton<SceneManager> {

	friend Singleton<SceneManager>;
public:
	enum scenes { Hub_, Frog_, Tree_, Eye_, Tutorial_, Menu_, Controls_, PauseMenu_ ,EscapeScene_};
	~SceneManager();
	void update();
	//inicializa la escena en la que está
	void init();
	void render();
	void changePauseState(bool state) { isPauseActive_ = state; };
	scenes getScene() { return actScene; }
	scenes getPreviousScene() { return previousScene_; }
	void changeScene(scenes s_=Hub_);
	void changeFrogEssenceState(bool active);
	void changeTreeEssenceState(bool active);
	void changeEyeEssenceState(bool active);
	void FrogSceneState(bool active);
	void TreeSceneState(bool active);
	void EyeSceneState(bool active);
	void changeStatePlayerInBoss(bool active);
	void activatePause();
	void decideScene();
	//activa la primera escena a la que se puede acceder desde el hub
	void setFirstSceneAble();
private:
	SceneManager();
	//decide a que escena hay que cambiar
	scenes actScene;
	scenes previousScene_;
	std::vector <Scene*> sceneList;
	Hub* h_;
	FrogScene* f_;
	TreeScene* t_;
	FinalBossScene* fin_;
	TutorialScene* tut_;
	MenuScene* menu_;
	ControlsScene* controls_;
	PauseMenu* pauseMenu_;
	EscapeScene* ecapesc_;
	bool frogEssenceObtained_, treeEssenceObtained_, eyeEssenceObtained_;
	bool hubAssetsChargeds_, playerInBossFight;
	bool isPauseActive_;
};
	// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
	inline SceneManager& sC() {
		return *SceneManager::instance();
	}

