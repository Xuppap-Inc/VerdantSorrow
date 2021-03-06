#pragma once
#include "BaseMenu.h"
class CreditsScene :
    public BaseMenu
{
private:
	std::vector<std::string> buttonNames_ = { "back" };
	std::vector<Transform*> buttonPositions_;
	std::vector<Entity*> buttonPoperties_; //Vector que guarda los botones como entidad (para el cambio de alfa)
	float delay_; //Delay del mando
	float lastUpdate_;
	int controllerIdex_;
	void onButtonClicked(int index) override; //Virtual puro cada menu hace cosas diferentes segun sus botones
	void generateAllButtons() override; //Virtual puro, cada menu dibuja sus botones de diferentes formas
	void background();
	Music* musica_;
	int musicVolume_;
public:
	CreditsScene();
	~CreditsScene() {};
	void init() override;
	void update() override; //Los menus pueden hacer cosas difrentes en el update
};

