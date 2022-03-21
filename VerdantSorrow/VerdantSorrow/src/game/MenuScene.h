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
private:
	std::vector<Transform *> buttonPositions;
};

