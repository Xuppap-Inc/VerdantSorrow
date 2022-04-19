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

void Parallax::AddLayer(Texture* tex, float scrollratio,int height,int width,int y)
{
	auto e=mngr_->addEntity();
	e->addComponent<Transform>(Vector2D(0,y),Vector2D(0,0),width,height,0);
	auto s=e->addComponent<RenderParallax>(tex,scrollratio);

	e->addToGroup(ecs::_BACKGROUND_1_GRP);
	auto es = mngr_->addEntity();
	es->addComponent<Transform>(Vector2D(-width, y), Vector2D(0, 0), width, height, 0);
	auto l=es->addComponent<RenderParallax>(tex, scrollratio);

	es->addToGroup(ecs::_BACKGROUND_1_GRP);
	auto ed = mngr_->addEntity();
	ed->addComponent<Transform>(Vector2D(width, y), Vector2D(0, 0), width, height, 0);
	auto v=ed->addComponent<RenderParallax>(tex, scrollratio);
	ed->addToGroup(ecs::_BACKGROUND_2_GRP);
	
}
