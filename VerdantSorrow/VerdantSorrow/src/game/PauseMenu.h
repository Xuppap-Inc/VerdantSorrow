#pragma once
#include "BaseMenu.h"
#include "../sdlutils/SDLUtils.h"
class PauseMenu :public BaseMenu
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
	void init() override;
	void update()override;
	void onButtonClicked(int index)override; //Versión raton
	void generateAllButtons() override;

	void controlVolume(bool turnUp); //Booleano para ver si sube o baja el volumen
	void createImages(float x, float y, float w, float h, std::string imageName);
	

};

