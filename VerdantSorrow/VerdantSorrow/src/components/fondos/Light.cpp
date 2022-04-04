#include "Light.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"
#include "../Image.h"

Light::Light(Texture* tex, int x, int y, int w, int alpha, ecs::Manager* mngr) : tex_(tex), mngr_(mngr)
{
	auto light = mngr_->addEntity();

		light->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, w, 0.0f);
		Image* imag = light->addComponent<Image>(tex_);
		imag->setAlpha(alpha);
}

