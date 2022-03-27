#pragma once

#include <vector>
//#include "Scene.h"
#include "MenuScene.h"

class ControlsScene : public MenuScene
{
private:
	//Se usaran los vectores si se añaden mas botones o imagenes
	//std::vector<std::string> buttonNames = { "back" };
	//std::vector<std::string> imagesNames = { "keyboardControls" };
	std::vector<Transform*> buttonPositions_;
public:
	ControlsScene();
	~ControlsScene() {};
	void init();
	void background();
	//Se sobreescribe onButtonClicked, ya que los botones de este menu tienen una funcion diferente
	void onButtonClicked(int index)override; 
	void createImages(float x, float y, float w, float h, std::string controlsImage);
	void createText(std::string message);
};


