#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"

class ecs::Manager;
class Light
{
public:
	Light(Texture* tex, int x, int y, int w, ecs::Manager* mngr);

private:
	Texture* tex_;
	ecs::Manager* mngr_;
};




