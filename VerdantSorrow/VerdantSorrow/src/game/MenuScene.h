#pragma once

#include <vector>
#include "Scene.h"


class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene() {};
	void init();
	void background();
	void createButton(float x, float y,float w, float  h, std::string buttonImage);
	void update()override;
	void onButtonClicked(Vector2D buttonPos);
private:
	std::vector<Transform *> buttonPositions;
	std::vector<Entity*> allButtons;
};

