#include "Parallax.h"
#include "ShowAtOppositeSide.h"
#include <cassert>

#include "../../ecs/Entity.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include "../../game/Fondos/Capas.h"

Parallax::Parallax()
{
}

Parallax::~Parallax()
{
}

void Parallax::initComponent()
{
}

void Parallax::newLayer(Texture* tex, float vel)
{
	/*Capas* s = new Capas(tex,vel);
	layer.push_back(s);*/
}

void Parallax::update()
{
}
