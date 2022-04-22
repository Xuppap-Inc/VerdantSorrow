
#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/VirtualTimer.h"

class Transform;
class Texture;

class Image : public ecs::Component {
public:

	// This line expands to the following (see the defintion of
	// __CMPID_DECL__ en ecs.h):
	//
	//    constexpr static ecs::cmpId_type id = ecs::_IMAGE
	//
	__CMPID_DECL__(ecs::_IMAGE)

		Image();
	Image(Texture* tex);
	virtual ~Image();

	void setTexture(Texture* tex) {
		tex_ = tex;
	}
	Texture* getTexture() {
		return tex_;
	}
	void setAlpha(int num);

	void initComponent() override;
	void render() override;

	void setVisible(bool set);
	bool isVisible();

	void fadeIn();
	void fadeOut();
	void setColor(Uint8 r, Uint8 g, Uint8 b, int duration = -1);
	void update() override;
private:
	Transform* tr_;
	Texture* tex_;

	bool visible_;

	bool isFadingIn_;
	bool isFadingOut_;
	int alpha_;

	VirtualTimer colorTimer_;
	int colorDuration_ = 0;
	int red_ = 255, green_ = 255, blue_ = 255;
};

