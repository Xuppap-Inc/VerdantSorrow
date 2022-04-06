#include "Renderparallax.h"
#include <cassert>

#include "../Transform.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"



RenderParallax::RenderParallax(Texture* tex,float scrollratio):tex_(tex),tr_(nullptr),scrollratio_(scrollratio)
{
}

RenderParallax::~RenderParallax()
{

}

void RenderParallax::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}


void RenderParallax::render()
{
	SDL_Rect dest = build_sdlrect(0,0, sdlutils().width(), sdlutils().height());
	auto pos = tr_->getPos();
	auto cam = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>()->getPos()*scrollratio_ ;
//	auto player = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>()->getPos();
	SDL_Rect dest1 = build_sdlrect(pos.getX() - cam.getX(), pos.getY()-cam.getY(), tr_->getWidth(), tr_->getHeight());

	


	assert(tex_ != nullptr);
	tex_->render(dest1, tr_->getRot());
	/*Vector2D destPos = Vector2D(tr_->getPos().getX() - (tr_->getWidth() * 0.5f), tr_->getPos().getY() - (tr_->getHeight() * 0.5f));
	SDL_Rect dest = build_sdlrect(destPos.getX() - (cam.getY() * scrollratio_), 0,
		tr_->getWidth(), tr_->getHeight());
       tex_->render(dest, 0);*/
}
