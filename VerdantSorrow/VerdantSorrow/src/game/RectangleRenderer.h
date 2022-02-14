#pragma once
#include "RenderComponent.h"
#include <SDL_pixels.h>

class RectangleRenderer : public RenderComponent {
public:
	RectangleRenderer();
	RectangleRenderer(SDL_Color color);
	virtual ~RectangleRenderer();

	virtual void render(Container* o, float r) override;

private:
	SDL_Color color_;
};

