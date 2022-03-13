#include "VFX.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "FramedImage.h"

VFX::VFX(int frames) : anim_(), frames_(frames)
{
	
}

VFX::~VFX()
{
}


void VFX::initComponent()
{
	anim_ = ent_->getComponent<FramedImage>();

	assert(anim_ != nullptr);
}


void VFX::update() {
	std::cout << anim_->getFrameNum() << std::endl;
	if (anim_->getFrameNum() == frames_-2) {
		ent_->setAlive(false);
	}
}