#pragma once
#include "../ecs/Component.h"

class Texture;

class Transform;
class FramedImage;

#include "../sdlutils/SDLUtils.h"

class VFX : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_VFX)


		VFX(int frames);
	virtual ~VFX();

	void initComponent() override;
	void update();
private:
	FramedImage* anim_;

	int frames_;
};



