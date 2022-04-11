#pragma once
#include "MenuScene.h"
#include "../sdlutils/SDLUtils.h"
class PauseMenu :public MenuScene
{
private:
	std::vector<std::string> buttonNames_ = { "volume","resume","menu","quit" };
	//std::vector<std::string> imagesNames = { "keyboardControls" };
	std::vector<Transform*> buttonPositions_;
	int controllerIndex_;
	float delay_; //Delay del mando
	float lastUpdate_;
	Music* musicaTest_;
	int currentVolume_;
	bool changeSc_;

public:
	PauseMenu();
	~PauseMenu() {};
	void init();
	//void background();
	//Se sobreescribe onButtonClicked, ya que los botones de este menu tienen una funcion diferente
	void onButtonClicked(int index) override;
	void generateAllButtons() override;
	void handleControllerInput() override;
	void changeButton(int numMoves) override;
	void controlVolume();
};

