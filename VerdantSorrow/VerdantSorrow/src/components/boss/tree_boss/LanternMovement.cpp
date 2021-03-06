#include "LanternMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../components/Transform.h"
#include "../../fondos/ParticleSystem.h"

LanternMovement::LanternMovement() :treeTr_(), lanternTr_(), active(true), rightSide(false)
{
}

LanternMovement::~LanternMovement()
{
}

void LanternMovement::initComponent()
{
	lanternTr_ = ent_->getComponent<Transform>();
	treeTr_ = mngr_->getHandler(ecs::_TREEBOSS)->getComponent<Transform>();
	assert(lanternTr_ != nullptr);
}

void LanternMovement::update()
{
	auto treePos = treeTr_->getPos();
	
	if (active) lanternTr_->getPos().set(Vector2D(treePos.getX() + treeTr_->getWidth() / 2 - lanternTr_->getWidth() / 2, treePos.getY() + treeTr_->getHeight() / 2 - lanternTr_->getHeight()));
}

bool LanternMovement::isActive()
{
	return active;
}

void LanternMovement::setActive(bool set)
{
	active = set;
}

void LanternMovement::moveToSide()
{

	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_amarilla"), mngr_);
	particlesys->createParticlesLanternMove(10, lanternTr_->getPos().getX() + (lanternTr_->getWidth() / 2), sdlutils().height() - 50);
	
	ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("particula_simbolo2"), mngr_);
	particlesys2->createParticlesLanternMove(10, lanternTr_->getPos().getX() + (lanternTr_->getWidth() / 2), sdlutils().height() - 50);
	
	int x;
	if (rightSide) x = sdlutils().width() - 100 - lanternTr_->getHeight() / 2;
	else x = 100;

	lanternTr_->getPos().set(Vector2D(x, sdlutils().height() - lanternTr_->getHeight() - GROUND_HEIGHT));

	rightSide = !rightSide;

}
