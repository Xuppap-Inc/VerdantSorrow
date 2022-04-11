#pragma once

#include <vector>
#include "Scene.h"

class MenuScene : public Scene
{
private:
	//Vector con los nombres de los botones aignados en "menu.json"
	//std::vector<std::string> buttonNames = {  "new_Game","continue","load", "settings", "controls","quit" };
	std::vector<std::string> buttonNames_= {  "new_Game","continue","controls","quit" };
	std::vector<Transform *> buttonPositions_; //Vector que guarda los transforms de los botones
	std::vector<Entity*> buttonPoperties_; //Vector que guarda los botones como entidad (para el cambio de alfa)
	int mouseIndex_;
	int controllerIndex_;
	float delay_; //Delay del mando
	float lastUpdate_;
	bool changeSc_;
public:
	MenuScene();
	~MenuScene() {};
	void init();
	void background();
	virtual void createButton(float x, float y,float w, float  h, std::string buttonImage);
	void update()override;
	virtual void onButtonClicked(int index); //Versión raton
	virtual void generateAllButtons();
	void handleInput();
	void handleMouseInput();
	virtual void handleControllerInput();
	void selectButton(int index);
	virtual void changeButton(int numMoves);
	void deselectButton(int index);
	
};

