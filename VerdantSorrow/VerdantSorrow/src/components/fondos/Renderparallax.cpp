#include "Renderparallax.h"
#include <cassert>

#include "../Transform.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"



RenderParallax::RenderParallax(Texture* tex,float scrollratio):tex_(tex),distance(0),tr_(nullptr),scrollratio_(scrollratio),startpos(0)
{
}

RenderParallax::~RenderParallax()
{
}

void RenderParallax::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	startpos = tr_->getPos().getX();
	distance = startpos;
}


void RenderParallax::render()
{
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getWidth(), tr_->getHeight());

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	dest.x *= sW;
	dest.w *= sW;
	dest.y *= sH;
	dest.h *= sH;

	assert(tex_ != nullptr);
	tex_->render(dest, tr_->getRot());
}

void RenderParallax::update()
{
	
	/*float temp = (mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>()->getPos().getX() * (1 - scrollratio_));
	float dist = (mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>()->getPos().getX() * (scrollratio_));
	auto& pos = tr_->getPos();
	pos.set(startpos + dist, tr_->getPos().getY());
	if (temp >= startpos + tr_->getWidth()) {
		startpos += ( 3*tr_->getWidth());
	}
	else if (temp <= startpos - tr_->getWidth()) {

		startpos -= (3*tr_->getWidth());
	}*/

	int dir = (mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerCtrl>()->getMovementDir());
	auto& pos = tr_->getPos();
	//float dist = (mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>()->getPos().getX() * (scrollratio_));
	if (dir == 1) {
		pos.set(pos.getX() - scrollratio_, tr_->getPos().getY());
		distance -= scrollratio_;
	}
	else if(dir==-1) {
		pos.set(pos.getX() +scrollratio_, tr_->getPos().getY());
		distance += scrollratio_;
	}

	if (distance >= startpos + tr_->getWidth()) {
		startpos = (3 * tr_->getWidth());
		pos.set(3 * tr_->getWidth(), tr_->getPos().getY());
		distance = 0;
	}
	else if (distance <= startpos - tr_->getWidth()) {

		startpos = -(3 * tr_->getWidth());
		pos.set(startpos, tr_->getPos().getY());
		distance = 0;
	}
		
	
}
