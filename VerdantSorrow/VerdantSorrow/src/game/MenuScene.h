#pragma once

#include <vector>
#include "Scene.h"

class MenuScene : public Scene
{
private:
	//Vector con los nombres de los botones aignados en "menu.json"
	std::vector<std::string> buttonNames = {  "new_Game","continue","load", "settings", "controls","quit" };
	std::vector<Transform *> buttonPositions_;
	int controllerIndex_=-1;
	bool allowChange_=true;
	float delay_=250;
	float lastUpdate_=0;
public:
	MenuScene();
	~MenuScene() {};
	void init();
	void background();
	virtual void createButton(float x, float y,float w, float  h, std::string buttonImage);
	void update()override;
	virtual void onButtonClicked(int index); //Versión raton
	//virtual void onButtonSelected(int index); //Version joystick
	void generateAllButtons();
	void handleInput();
	void changeStyle();
	void changeButton(int numMoves);
};

