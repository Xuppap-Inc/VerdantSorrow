#include "ParticleSystem.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../utils/Vector2D.h"
#include "../Image.h"
#include "ShowAtOppositeSide.h"
#include "Disolve.h"
#include "Target.h"

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
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(100, 500)) / 100) * dir, ((double)(sdlutils().rand().nextInt(-500, 100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		particle->addComponent<Disolve>(400);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		imag->setColor(sdlutils().rand().nextInt(220, 255), sdlutils().rand().nextInt(220, 255), sdlutils().rand().nextInt(220, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesEssence(int numpart, int x, int y, Transform* target)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(30, 50);
		int dirX = 1;
		if (sdlutils().rand().nextInt(0, 2) == 0) dirX = -1;
		int dirY = 1;
		if (sdlutils().rand().nextInt(0, 2) == 0) dirY = -1;
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(0, 500)) / 100) * dirX, ((double)(sdlutils().rand().nextInt(0, 500)) / 100) * dirY);
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		particle->addComponent<Target>(750, target);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

