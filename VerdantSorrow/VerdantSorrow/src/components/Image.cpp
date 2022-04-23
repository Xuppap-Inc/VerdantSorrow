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

	colorTimer_ = new VirtualTimer();
	mngr_->addTimer(colorTimer_);
}

Image::Image(Texture* tex) :
	tr_(), tex_(tex), visible_(true) {

	colorTimer_ = new VirtualTimer();
	mngr_->addTimer(colorTimer_);
}

Image::~Image() {
}

void Image::setAlpha(int num)
{
	alpha_ = num;
	assert(alpha_ >= 0 && alpha_ <= 255);
	tex_->setAlpha(alpha_);
}

void Image::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);

	colorTimer_->reset();
}

void Image::render() {

	if (visible_) {
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
}

void Image::setVisible(bool set)
{
	visible_ = set;
}

bool Image::isVisible()
{
	return visible_;
}

void Image::fadeIn()
{
	isFadingOut_ = false;
	isFadingIn_ = true;
}

void Image::fadeOut()
{
	isFadingIn_ = false;
	isFadingOut_ = true;
}

void Image::setColor(Uint8 r, Uint8 g, Uint8 b, int duration) {
	red_ = r;
	green_ = g;
	blue_ = b;
	colorDuration_ = duration;
	colorTimer_->reset();
}

void Image::update() {
	if (colorDuration_ != -1)
	{
		if (colorTimer_->currTime() >= colorDuration_) {
			red_ = 255;
			green_ = 255;
			blue_ = 255;
		}
	}
	getTexture()->setColor(red_, green_, blue_);

	if (isFadingIn_) {
		alpha_++;
		setAlpha(alpha_);
	}
}