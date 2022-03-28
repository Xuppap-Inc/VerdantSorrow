#include "ParticleSystem.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"
#include "../Image.h"
#include "ShowAtOppositeSide.h"

ParticleSystem::ParticleSystem(Texture* tex, int numpart,ecs::Manager* mngr):numpart_(numpart),tex_(tex),mngr_(mngr)
{
}

void ParticleSystem::createParticles()
{
	for (int i = 0; i < numpart_; i++) {
		auto particle = mngr_->addEntity();
	
		auto posx = sdlutils().rand().nextInt(0, sdlutils().width() + 1);
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height() + 1);
		auto width= sdlutils().rand().nextInt(30,40);
		auto vel = Vector2D(sdlutils().rand().nextInt(-5, 6), sdlutils().rand().nextInt(-5, 6));
		particle->addComponent<Transform>(Vector2D(posx,posy),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(120);
		particle->addComponent<ShowAtOpposideSide>();
	}
}

