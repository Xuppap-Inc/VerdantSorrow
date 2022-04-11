#pragma once


class Texture;
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class Parallax
{
public:
	Parallax(ecs::Manager *mngr);
	void AddLayer(Texture* tex, float scrollratio, int height, int width,int y);

private:
	ecs::Manager* mngr_;

};

