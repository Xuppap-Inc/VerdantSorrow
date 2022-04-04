#pragma once
#include "MenuScene.h"
#include "../sdlutils/SDLUtils.h"
class PauseMenu :public MenuScene
{
private:
	std::vector<std::string> buttonNames = { "volume","resume","menu","quit" };
	//std::vector<std::string> imagesNames = { "keyboardControls" };
	std::vector<Transform*> buttonPositions_;
	int controllerIndex_ = -1;
	float delay_ = 250; //Delay del mando
	float lastUpdate_ = 0;
	Music* musicaTest_;
	int currentVolume_=0;

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

