#include "RootMovement.h"

#include "../../../Transform.h"
#include "../../../../ecs/Entity.h"
#include "../../../../ecs/Manager.h"
#include "../../../fondos/ParticleSystem.h"


RootMovement::RootMovement() : tr_(), col_(), speed_(1), waitTimer_(), state_(APPEARING)
{
}
RootMovement::~RootMovement()
{
}

void RootMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	col_ = ent_->getComponent<RectangleCollider>();
	col_->setActive(false);
	waitTimer_ = mngr_->addTimer();

	auto& vel = tr_->getVel();
	vel = new Vector2D(0, -speed_);
}

void RootMovement::update()
{
	if (state_ == DYING)
	{
		if (waitTimer_->currTime() >= WAIT_TO_DIE) {

			//particles
			/*ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_simbolo1"), mngr_);
			particlesys->createParticlesRootsDie(3, tr_->getPos().getX() + (tr_->getWidth() / 2));
			ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("luz_naranja"), mngr_);
			particlesys2->createParticlesRootsDie(9, tr_->getPos().getX() + (tr_->getWidth() / 2));*/

			ent_->setAlive(false);
		}
	}
	else if (state_ == GROWING)
	{
		//si ha llegado al borde de la pantalla
		if (tr_->getPos().getY() <= 100) {
			speed_ = 0;
			waitTimer_->reset();

			col_->setActive(false);

			state_ = DYING;

			auto& vel = tr_->getVel();
			vel = new Vector2D(0, -speed_);
		}
	}
	else if (state_ == APPEARING)
	{
		//si ha llegado a la posicion en la que comienza a crecer deprisa
		if (tr_->getPos().getY() < sdlutils().height() - MAX_HEIGH_BEFORE_GROWING) {
			speed_ = 50;
			col_->setActive(true);

			auto& vel = tr_->getVel();
			vel = new Vector2D(0, -speed_);

			state_ = GROWING;
		}

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_tierra"), mngr_);
		particlesys->createParticlesRoots(1, tr_->getPos().getX() + (tr_->getWidth() / 2), sdlutils().height() - 50);

		ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("particula_hoja"), mngr_);
		particlesys2->createParticlesRoots(1, tr_->getPos().getX() + (tr_->getWidth() / 2), sdlutils().height() - 50);
	}
}