#pragma once
#include "../ecs/Component.h"
#include <SDL_pixels.h>

class Transform;
class RectangleRenderer : public ecs::Component {
public:
	__CMPID_DECL__(ecs::_RENDERER)
	RectangleRenderer();
	RectangleRenderer(SDL_Color color);
	virtual ~RectangleRenderer();
	void initComponent() override;

	virtual void render() override;

private:
	Transform* tr_;
	SDL_Color color_;
};

