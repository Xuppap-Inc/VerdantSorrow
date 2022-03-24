#include "Attack.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"
#include "../boss/BossAtributos.h"
#include "../boss/frog_boss/FlyHp.h"
#include "../../ecs/Manager.h"
#include "../VFX.h"
#include "../tutorial/TutorialFly.h"

Attack::Attack(float width, float height, CollisionManager* colManager) :
	tr_(nullptr), RectangleCollider(width, height), attackDuration(300),
	attackCoolDown(300), lastAttack(), newAttack(false), finished_(true)
{
	setActive(false);
	colMan_ = colManager;
}

Attack::~Attack()
{
}

void Attack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	anim_ = ent_->getComponent<FramedImage>();
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(tr_ != nullptr, collider_ != nullptr && attrib_ != nullptr);
}

void Attack::update()
{
	auto& ihdlr = ih();

	if (isActive()) { //si esta activo, se coloca en la posicion correspondiente

		setPosition();

		if (sdlutils().currRealTime() >= lastAttack + attackDuration) {
			
			setActive(false);
			finished_ = true;
		}
	}
	else {
		if (ihdlr.keyDownEvent()) {//si no esta activo, comprueba si se puede activar (cooldown y j presionada)
			if (ihdlr.isKeyDown(SDLK_j)) {
				if (sdlutils().currRealTime() >= lastAttack + attackDuration + attackCoolDown) {

					if (attrib_->isOnGround()) {
						anim_->repeat(true);
						anim_->changeanim(&sdlutils().images().at("Chica_AtkFloor"), 3, 3, 1000, 9, "Chica_AtkFloor");
					}
					else {
						anim_->repeat(true);
						anim_->changeanim(&sdlutils().images().at("Chica_AtkAir"), 3, 5, 100, 15, "Chica_AtkAir");
					}

					SoundEffect* s = &sdlutils().soundEffects().at("sfx_chica_attack2");
					s->play();
					newAttack = true;

					setActive(true);
					lastAttack = sdlutils().currRealTime();
					setPosition();//si no setamos la posicion aqui, se renderizara un frame del ataque en una posicion que no debe
				}
			}
		}
	}
	//Colisiones (con boss)
	if (isActive() && colMan_->hasCollisions(this)) {

		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(this);

		for (auto c : colliders) {


			if (c->isActive() && c->isTrigger() ) 
			{
				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				if (bA != nullptr && newAttack) {
					SoundEffect* s = &sdlutils().soundEffects().at("sfx_chica_attack1");
					s->setChannelVolume(70);
					s->play();

					auto VFXEnt = mngr_->addEntity();
					auto VFXTr = VFXEnt->addComponent<Transform>();
					VFXTr->init(Vector2D(tr_->getPos().getX() - 125, tr_->getPos().getY()), Vector2D(), 400, 200, 0.0f);
					VFXEnt->addComponent<FramedImage>(&sdlutils().images().at("vfx_attack"), 1, 6, (1000 / 30) * 6, 6, "vfx");
					VFXEnt->addComponent<VFX>(6);
					 
					bA->setDamage(0.6f);
					tr_->getVel().setY(0);
					newAttack = false;

					finished_ = false;
				}
				FlyHp* fHP = ent->getComponent<FlyHp>();
				if (fHP != nullptr) {
					fHP->receiveHit();
				}
				TutorialFly* tFl = ent->getComponent<TutorialFly>();
				if (tFl != nullptr) {
					tFl->receiveHit();
				}
			}
		}
	}
}

bool Attack::hasFinished()
{
	return finished_;
}

void Attack::setPosition()
{
	int playerMovementDir = ent_->getComponent<PlayerCtrl>()->getMovementDir();

	Vector2D contPos = tr_->getPos();

	if (playerMovementDir >= 0)
		pos_ = Vector2D(contPos.getX() + tr_->getWidth(), contPos.getY());
	else 
		pos_ = Vector2D(contPos.getX() - width_, contPos.getY());
}
