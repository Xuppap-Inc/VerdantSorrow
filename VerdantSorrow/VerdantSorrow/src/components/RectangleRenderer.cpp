#include "RectangleRenderer.h"

#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"

RectangleRenderer::RectangleRenderer() :
	RectangleRenderer(build_sdlcolor(0xffffffff)) {

}

RectangleRenderer::RectangleRenderer(SDL_Color color):visible_(true) ,
	tr_(nullptr), color_(color) {
}

RectangleRenderer::~RectangleRenderer()
{
}

void RectangleRenderer::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void RectangleRenderer::setVisible(bool set)
{
	visible_ = set;
}

void RectangleRenderer::render()
{
	if (visible_) {
		auto& pos = tr_->getPos();
		auto width = tr_->getWidth();
		auto height = tr_->getHeight();

		SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(color_));

		SDL_Rect rect = build_sdlrect(pos.getX() , pos.getY() , width, height);
		
		//escalado pantalla
		auto sW = mngr_->getWindowScaleWidth();
		auto sH = mngr_->getWindowScaleHeight();

		rect.x *= sW;
		rect.w *= sW;
		rect.y *= sH;
		rect.h *= sH;

		SDL_RenderFillRect(sdlutils().renderer(), &rect);
	}
	
}
