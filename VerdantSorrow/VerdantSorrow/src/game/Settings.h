#pragma once
#include "BaseMenu.h"
class Settings :
    public BaseMenu
{
private:
	std::vector<std::string> buttonNames_ = { "back", "foto480","foto144","foto1080","foto720" };
	std::vector<Transform*> buttonPositions_;
	std::vector<Entity*> buttonPoperties_; //Vector que guarda los botones como entidad (para el cambio de alfa)
	float delay_; //Delay del mando
	float lastUpdate_;
	int controllerIdex_;

	
	void onButtonClicked(int index) override; //Virtual puro cada menu hace cosas diferentes segun sus botones
	void generateAllButtons() override; //Virtual puro, cada menu dibuja sus botones de diferentes formas
	void background();
public:
	Settings();
	~Settings() {};
	void init() override;
	void update() override; //Los menus pueden hacer cosas difrentes en el update
};

