#include "Renderparallax.h"
#include <cassert>

#include "../Transform.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"



RenderParallax::RenderParallax(Texture* tex) :tex_(tex),tr_(nullptr)
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
	SDL_Rect dest = build_sdlrect(0,0, sdlutils().width(), tr_->getHeight());
	auto pos = tr_->getPos();
	auto cam=
	pos = (pos - mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>()->getPos())*tr_->getVel().getX();
	SDL_Rect p= build_sdlrect(pos, tr_->getWidth(), tr_->getHeight());
	tex_->render(dest, p, tr_->getRot(), nullptr);
}
