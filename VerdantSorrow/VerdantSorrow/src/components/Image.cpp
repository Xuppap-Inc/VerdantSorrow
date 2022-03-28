#include "Image.h"
// This file is part of the course TPV2@UCM - Samir Genaim



#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"
#include "../ecs/Manager.h"

Image::Image() :
	tr_(), tex_(), visible_(true) {
}

Image::Image(Texture* tex) :
	tr_(), tex_(tex), visible_(true) {
}

Image::~Image() {
}

void Image::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void Image::render() {

	if (visible_) {
		Vector2D v = tr_->getPos() - mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>()->getPos();
		SDL_Rect dest = build_sdlrect(v, tr_->getWidth(), tr_->getHeight());

		assert(tex_ != nullptr);
		tex_->render(dest, tr_->getRot());
	}
}

void Image::setVisible(bool set)
{
	visible_ = set;
}

bool Image::isVisible()
{
	return visible_;
}
