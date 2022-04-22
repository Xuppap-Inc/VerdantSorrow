#include "waveMovement.h"

#include "../../Transform.h"
#include "../../../ecs/Entity.h"
#include "../../fondos/ParticleSystem.h"

WaveMovement::WaveMovement(Vector2D dir, float speed, bool particles) : tr_(), dir_(dir), speed_(speed), lastTime_(0), particles_(particles)
{
}
WaveMovement::~WaveMovement()
{
}

void WaveMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	lastTime_ = sdlutils().currRealTime();
}


void WaveMovement::update()
{
	auto& vel = tr_->getVel();
	auto pos = tr_->getPos();

	if (dir_.magnitude() != 0)
		vel.set(dir_.normalize() * speed_);

	if (pos.getX() < 0 - tr_->getWidth() || pos.getX() > sdlutils().width()
		|| pos.getY() < -tr_->getHeight() || pos.getY() > sdlutils().height()) {
		ent_->setAlive(false);
	}

	if (particles_) {
		// Onda horizontal
		if (vel.getY() == 0) {
			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_tierra"), mngr_);
			particlesys->createParticlesWave(1, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + tr_->getHeight());
		}
		// Bola de fuego
		else
		{
			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_morado"), mngr_);
			particlesys->createParticlesFireball(2, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + tr_->getHeight()/2);
		}
	}
}