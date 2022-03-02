#include "RectangleRenderer.h"

#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"

RectangleRenderer::RectangleRenderer() :
	RectangleRenderer(build_sdlcolor(0xffffffff)) {

}

RectangleRenderer::RectangleRenderer(SDL_Color color) :
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

void RectangleRenderer::render()
{
	auto& pos = tr_->getPos();
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();

	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(color_));

	SDL_Rect rect = build_sdlrect(pos.getX(), pos.getY(), width, height);

	SDL_RenderFillRect(sdlutils().renderer(), &rect);
	
}
