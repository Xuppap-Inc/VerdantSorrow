
#pragma once
#include "../ecs/Component.h"

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
	void setAlpha(int num);

	void initComponent() override;
	void render() override;

	void setVisible(bool set);
	bool isVisible();

private:
	Transform* tr_;
	Texture* tex_;

	bool visible_;
};

