#pragma once

#include <vector>
#include "Scene.h"

class MenuScene : public Scene
{
private:
	//Vector con los nombres de los botones aignados en "menu.json"
	std::vector<std::string> buttonNames = {  "new_Game","continue","load", "settings", "controls","quit" };
	std::vector<Transform *> buttonPositions_;
	std::vector<Entity*> buttonPoperties_;
	int mouseIndex_ = -1;
	int controllerIndex_=-1;
	bool canMoveBack_=false;
	float delay_=250;
	float lastUpdate_=0;
	int buttonW_=200, buttonH_=80;
	int offsetX_ = 50;
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
	void selectButton(int index);
	void changeButton(int numMoves);
	void deselectButton(int index);
};

