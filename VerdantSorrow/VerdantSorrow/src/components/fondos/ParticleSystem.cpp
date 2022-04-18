#include "ParticleSystem.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"
#include "../Image.h"
#include "ShowAtOppositeSide.h"
#include "Disolve.h"

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

void ParticleSystem::createParticlesDirt(int numpart, int dir, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(10, 20);
		auto vel = Vector2D(sdlutils().rand().nextInt(1, 6) * dir, sdlutils().rand().nextInt(-5, 0));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		particle->addComponent<Disolve>(400);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		imag->setColor(sdlutils().rand().nextInt(220, 255), sdlutils().rand().nextInt(220, 255), sdlutils().rand().nextInt(220, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

