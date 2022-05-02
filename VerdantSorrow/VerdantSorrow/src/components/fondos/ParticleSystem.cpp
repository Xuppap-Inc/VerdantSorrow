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

void ParticleSystem::disolveParticles()
{
	for (int i = 0; i < parts.size(); i++) {
		parts[i]->addComponent<Disolve>(5000);
		parts[i]->getComponent<Image>()->fadeOut();
	}
}
void ParticleSystem::targetParticles(Transform* target)
{
	for (int i = 0; i < parts.size(); i++) {
		parts[i]->addComponent<Target>(sdlutils().rand().nextInt(100, 1000), target);
	}
}

void ParticleSystem::createParticlesAsh(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);

		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height());
		auto width= sdlutils().rand().nextInt(3,11);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-300, 300)) / 100), ((double)(sdlutils().rand().nextInt(-300, 300)) / 100));
		particle->addComponent<Transform>(Vector2D(posx,posy),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		imag->setColor(sdlutils().rand().nextInt(100, 200), sdlutils().rand().nextInt(100, 200), sdlutils().rand().nextInt(100, 200));
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_BACKGROUND_2);
	}
}

void ParticleSystem::createParticlesMenu(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);
	
		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height());
		auto width= sdlutils().rand().nextInt(30, 90);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-300, 300)) / 100), ((double)(sdlutils().rand().nextInt(-600, -100)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, posy), vel, width, width, 0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(255);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_UI);
	}
}

void ParticleSystem::createParticlesMenu2(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);
	
		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height());
		auto width= sdlutils().rand().nextInt(40, 240);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-100, 100)) / 100), ((double)(sdlutils().rand().nextInt(-100, -50)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, posy), vel, width, width, 0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_BACKGROUND_1);
	}
}

void ParticleSystem::createParticlesHub(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);

		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height());
		auto width = sdlutils().rand().nextInt(30, 90);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-300, 300)) / 100), ((double)(sdlutils().rand().nextInt(-600, -100)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, posy), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesDandellion(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);
	
		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto width= sdlutils().rand().nextInt(40, 80);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-300, 300)) / 100), ((double)(sdlutils().rand().nextInt(-300, 300)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, sdlutils().height()),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(150);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createOverlayParticlesDandellion(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);

		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto width= sdlutils().rand().nextInt(300, 500);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-100, 100)) / 100), ((double)(sdlutils().rand().nextInt(-100, 100)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, sdlutils().height()),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_FRONT);
	}
}

void ParticleSystem::createBackgroundParticlesSymbols(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);

		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto posy = sdlutils().rand().nextInt(0, sdlutils().height());
		auto width= sdlutils().rand().nextInt(200, 1000);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-20, 20)) / 100), ((double)(sdlutils().rand().nextInt(-20, 20)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, posy),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_BACKGROUND_1);
	}
}

void ParticleSystem::createParticlesRain(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);

		auto posx = sdlutils().rand().nextInt(0, sdlutils().width());
		auto width= sdlutils().rand().nextInt(20, 40);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-80, -20)) / 100), ((double)(sdlutils().rand().nextInt(800, 1200)) / 100));
		particle->addComponent<Transform>(Vector2D(posx, 0),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesWind(int numpart)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();
		parts.push_back(particle);

		int dir = 1;
		if (sdlutils().rand().nextInt(0, 2) == 1) dir = -1;

		auto width= sdlutils().rand().nextInt(40, 80);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(600, 1200)) / 100) * dir, ((double)(sdlutils().rand().nextInt(-200, 200)) / 100));
		particle->addComponent<Transform>(Vector2D(0, 0),vel,width,width,0.0f);
		Image *imag=particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<ShowAtOpposideSide>();
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesRoll(int numpart, int dir, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(10, 20);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-50, 500)) / 100) * dir, ((double)(sdlutils().rand().nextInt(-500, 100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(300, 500));
		imag->setColor(sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesSmash(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		int dir = 1;
		if (sdlutils().rand().nextInt(0, 2) == 1) dir = -1;
		auto width = sdlutils().rand().nextInt(5, 50);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(100, 500)) / 100) * dir, ((double)(sdlutils().rand().nextInt(-700, 100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(500, 1000));
		imag->setColor(sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesWave(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		x += (sdlutils().rand().nextInt(-100, 100));

		auto width = sdlutils().rand().nextInt(5, 40);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-200, 200)) / 100), ((double)(sdlutils().rand().nextInt(-700, -100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(200, 500));
		imag->setColor(sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesFireball(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		x += (sdlutils().rand().nextInt(-50, 50));
		y += (sdlutils().rand().nextInt(-50, 50));

		auto width = sdlutils().rand().nextInt(5, 40);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-300, 300)) / 100), ((double)(sdlutils().rand().nextInt(-600, 100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(200, 600));
		imag->setColor(sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesFire(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(10, 100);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-200, 200)) / 100), ((double)(sdlutils().rand().nextInt(-500, -100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(100);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(300, 500));
		imag->setColor(sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesRoots(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		int dir = 1;
		if (sdlutils().rand().nextInt(0, 2) == 1) dir = -1;
		auto width = sdlutils().rand().nextInt(10, 30);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(100, 200)) / 100) * dir, ((double)(sdlutils().rand().nextInt(-900, 100)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(200, 1500));
		imag->setColor(sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255), sdlutils().rand().nextInt(180, 255));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesRootsDie(int numpart, int x)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(25, 50);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-100, 100)) / 100), ((double)(sdlutils().rand().nextInt(-300, 300)) / 100));
		int y = sdlutils().rand().nextInt(100, sdlutils().height());
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Target>(sdlutils().rand().nextInt(500, 1000), mngr_->getHandler(ecs::_LANTERN)->getComponent<Transform>());
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}


void ParticleSystem::createParticlesLanternMove(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(10, 20);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-200, 200)) / 100), ((double)(sdlutils().rand().nextInt(-500,-200)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Target>(sdlutils().rand().nextInt(200, 1200), mngr_->getHandler(ecs::_LANTERN)->getComponent<Transform>());
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesLanternDamage(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(15, 25);
		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-300, 300)) / 100), ((double)(sdlutils().rand().nextInt(-400, 400)) / 100));
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Disolve>(sdlutils().rand().nextInt(100, 400));
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesHandMagic(int numpart, Transform* target)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(50, 100);

		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-100, 100)) / 100), ((double)(sdlutils().rand().nextInt(-700, -500)) / 100));
		particle->addComponent<Transform>(Vector2D(sdlutils().rand().nextInt(0, sdlutils().width()), sdlutils().height()), vel, width, width, 0.0f);
		particle->addComponent<Target>(sdlutils().rand().nextInt(400, 1000), target);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addToGroup(ecs::_PARTICLES_BACKGROUND_2);
	}
}

void ParticleSystem::createParticlesBossDie(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = sdlutils().rand().nextInt(30, 100);

		auto vel = Vector2D(((double)(sdlutils().rand().nextInt(-130, 130)) / 100), ((double)(sdlutils().rand().nextInt(-300, -10)) / 100));
		particle->addComponent<Transform>(Vector2D(x + sdlutils().rand().nextInt(-100, 100), y + sdlutils().rand().nextInt(-100, 100)), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Target>(sdlutils().rand().nextInt(500, 4000), mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>());
		particle->addToGroup(ecs::_PARTICLES_GRP);
	}
}

void ParticleSystem::createParticlesBossDieSymbol(int numpart, int x, int y)
{
	for (int i = 0; i < numpart; i++) {
		auto particle = mngr_->addEntity();

		auto width = 300;
		auto vel = Vector2D(0, -2);
		particle->addComponent<Transform>(Vector2D(x, y), vel, width, width, 0.0f);
		Image* imag = particle->addComponent<Image>(tex_);
		imag->setAlpha(200);
		particle->addComponent<Disolve>(5000);
		imag->fadeOut();
		particle->addToGroup(ecs::_PARTICLES_FRONT);
	}
}
