#include "ParticleSystem.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"
#include "../Image.h"
#include "ShowAtOppositeSide.h"

ParticleSystem::ParticleSystem(Texture* tex, ecs::Manager* mngr):tex_(tex),mngr_(mngr)
{
}

void ParticleSystem::createParticlesSnow(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
	
		auto posx = sdlutils().rand().nextInt(0, sdlutils().width() + 1);
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height() + 1);
		auto width= sdlutils().rand().nextInt(3,11);
		auto vel = Vector2D(sdlutils().rand().nextInt(-3, 4), sdlutils().rand().nextInt(-3, 4));
		particle->addComponent<Transform>(Vector2D(posx,posy),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

