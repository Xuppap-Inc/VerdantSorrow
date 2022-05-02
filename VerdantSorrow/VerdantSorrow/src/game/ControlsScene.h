#pragma once

#include <vector>
#include "BaseMenu.h"

class ControlsScene : public BaseMenu
{
private:
	//Se usaran los vectores si se añaden mas botones o imagenes
	std::vector<std::string> buttonNames_ = { "back" };
	std::vector<Transform*> buttonPositions_;
	std::vector<Entity*> buttonPoperties_; //Vector que guarda los botones como entidad (para el cambio de alfa)
	float delay_; //Delay del mando
	float lastUpdate_;
	int controllerIdex_;
	
public:
	ControlsScene();
	~ControlsScene() {};
	void init()override;
	void background();
	void update() override;
	//Se sobreescribe onButtonClicked, ya que los botones de este menu tienen una funcion diferente
	void onButtonClicked(int index)override; 
	void createImages(float x, float y, float w, float h, std::string controlsImage);
	void createText(float x, float y,std::string message);
	void generateAllButtons() override;

};


