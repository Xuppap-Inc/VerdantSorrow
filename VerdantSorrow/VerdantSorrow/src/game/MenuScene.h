#pragma once

#include <vector>
#include "Scene.h"

class MenuScene : public Scene
{
private:
	//Vector con los nombres de los botones aignados en "menu.json"
	std::vector<std::string> buttonNames = {  "new_Game","continue", "settings", "controls","quit" };
	std::vector<Transform *> buttonPositions_;
public:
	MenuScene();
	~MenuScene() {};
	void init();
	void background();
	void createButton(float x, float y,float w, float  h, std::string buttonImage);
	void update()override;
	void onButtonClicked(int index);
};

