#include "TutorialRootMovement.h"

#include "../Transform.h"
#include "../../ecs/Entity.h"
#include "../fondos/ParticleSystem.h"
#include "../../ecs/Manager.h"


TutorialRootMovement::TutorialRootMovement() : tr_(), col_(), speed_(0.25), rootSpawnTimer_()
{
}
TutorialRootMovement::~TutorialRootMovement()
{
}

void TutorialRootMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	col_ = ent_->getComponent<RectangleCollider>();
	col_->setActive(false);
	rootSpawnTimer_ = mngr_->addTimer();
}

void TutorialRootMovement::update()
{
	if (speed_ == 0) {
		col_->setActive(false);
		if (rootSpawnTimer_->currTime() > 700) {
			ent_->setAlive(false);
		}
	}
	else {
		if (tr_->getPos().getY() < sdlutils().height() - tr_->getHeight() + 100) {
			speed_ = 0;
			rootSpawnTimer_->reset();
		}
		else if (tr_->getPos().getY() < sdlutils().height() - 30) {
			speed_ = 50;
			col_->setActive(true);
			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_tierra"), mngr_);
			particlesys->createParticlesRoots(1, tr_->getPos().getX() + (tr_->getWidth() / 2), sdlutils().height() - 50);

			ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("particula_hoja"), mngr_);
			particlesys2->createParticlesRoots(1, tr_->getPos().getX() + (tr_->getWidth() / 2), sdlutils().height() - 50);
		}
		auto& vel = tr_->getVel();
		vel = new Vector2D(0, -speed_);
	}
}