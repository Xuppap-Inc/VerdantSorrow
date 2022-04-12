	#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"

class CollisionChecker;
class EscapeScene: public Scene
{
public: 
	EscapeScene() :Scene(), isAble(false) {}
	~EscapeScene() {};
	void init();

	void update()override;

	//start lo hace scene directamente
	void background();
	

	bool getAble();
	void setAble(bool a);
	void createLights();
private:
	bool isAble;
	Entity* player;
	Entity* FinalBossFace;
	CollisionChecker* colCheck_;
};

