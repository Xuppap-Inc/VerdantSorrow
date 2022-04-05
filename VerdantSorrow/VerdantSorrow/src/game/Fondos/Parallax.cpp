#include "Parallax.h"
#include "../../ecs/Manager.h"
#include "../../ecs/Entity.h"

#include "../../components/Transform.h"
#include "../../components/fondos/ShowAtOppositeSide.h"
#include "../../components/fondos/RenderParallax.h"
#include "../../ecs/Manager.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"



Parallax::Parallax(ecs::Manager* mngr):mngr_(mngr)
{
}

void Parallax::AddLayer(Texture* tex, float scrollratio,int height,int width)
{
	auto e=mngr_->addEntity();
	e->addComponent<Transform>(Vector2D(0,sdlutils().height()),Vector2D(scrollratio,0),width,height,0);
	e->addComponent<ShowAtOpposideSide>();
	e->addComponent<RenderParallax>(tex);
}
