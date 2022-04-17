#pragma once

#include <vector>
#include "BaseMenu.h"

class MenuScene : public BaseMenu
{
private:
	//Vector con los nombres de los botones aignados en "menu.json"
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
	
	void update()override;
	void onButtonClicked(int index)override; //Versión raton
	void generateAllButtons() override;
	void init() override;
	//void selectButton(int index);
	//void deselectButton(int index);
	//void background();
};

