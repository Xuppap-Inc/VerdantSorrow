#pragma once
#include "MenuScene.h"
#include "../sdlutils/SDLUtils.h"
class PauseMenu :public MenuScene
{
private:
	std::vector<std::string> buttonNames_ = { "leftVolume","rightVolume","resume","menu","quit" };
	std::vector<std::string> imagesNames_ = { "volume" };
	std::vector<Transform*> buttonPositions_;
	int controllerIndex_;
	float delay_; //Delay del mando
	float lastUpdate_;
	Music* musicaTest_;
	int currentVolume_;
	bool changeSc_;
	float varVolume_; //Cuanto varia el volumen respecto al volumen total al bajarlo o subirlo

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
	void controlVolume(bool turnUp); //Booleano para ver si sube o baja el volumen
	void createImages(float x, float y, float w, float h, std::string imageName);

};

