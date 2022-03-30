#include "Attack.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"
#include "../../ecs/Manager.h"
#include "../VFX.h"


Attack::Attack(float width, float height, CollisionManager* colManager) :
	tr_(nullptr), RectangleCollider(width, height), attackDuration(300),
	attackCoolDown(300), lastAttack(), newAttack_(false), finished_(true),
	recoveryTimer_(), recovery_(false), waitingForRecovery_(false),

	// INPUT
	attackKeys({ SDL_SCANCODE_J }),
	attackButtons({ SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER })
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

	if (waitingForRecovery_ && recoveryTimer_.currTime() >= TIME_UNTIL_RECOVERY) {
	
		waitingForRecovery_ = false;

		recoverAnim();
	}

	if (isActive()) { //si esta activo, se coloca en la posicion correspondiente

		setPosition();

		if (sdlutils().currRealTime() >= lastAttack + attackDuration) {

			setActive(false);
			finished_ = true;
		}
	}
	else {
		if (!mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerCtrl>()->isRolling() && sdlutils().currRealTime() >= lastAttack + attackDuration + attackCoolDown) {
			if (ihdlr.keyDownEvent() || ihdlr.controllerDownEvent()) {//si no esta activo, comprueba si se puede activar (cooldown y j presionada)

				bool attackThisFrame = false;

				// Keyboard
				int i = 0;
				while (i < attackKeys.size() && !ihdlr.isKeyDown(attackKeys[i])) i++;
				if (i < attackKeys.size()) attackThisFrame = true;
				// Controller
				i = 0;
				while (i < attackButtons.size() && !ihdlr.isControllerButtonDown(attackButtons[i])) i++;
				if (i < attackButtons.size()) attackThisFrame = true;

				if (attackThisFrame) {

					finished_ = false;

					//callback que llama a attack
					std::function<void()> attackCallback = [this]() { attack(); };

					//callback recovery
					std::function<void()> recoveryCallback = [this]() { activateRecoveryTimer(); };

					if (attrib_->isOnGround()) {
						anim_->repeat(false);
						anim_->changeanim(&sdlutils().images().at("Chica_AtkFloor"), 3, 3, 200, 9, "Chica_AtkFloor");

						//registra el evento en la animacion
						anim_->registerEvent(std::pair<int, std::string>(6, "Chica_AtkFloor"), attackCallback);

						anim_->registerEvent(std::pair<int, std::string>(8, "Chica_AtkFloor"), recoveryCallback);
					}
					else {
						anim_->repeat(false);
						anim_->changeanim(&sdlutils().images().at("Chica_AtkAir"), 3, 5, 100, 15, "Chica_AtkAir");

						//registra el evento en la animacion
						anim_->registerEvent(std::pair<int, std::string>(6, "Chica_AtkAir"), attackCallback);
					}
					lastAttack = sdlutils().currRealTime();
				}

			}
		}
	}
}

bool Attack::hasFinished()
{
	return finished_;
}

void Attack::setFinished(bool set)
{
	finished_ = set;
}

bool Attack::isNewAttack()
{
	return newAttack_;
}

void Attack::setNewAttack(bool set)
{
	newAttack_ = set;
}

bool Attack::hasFinishedRecovery()
{
	return !recovery_;
}

void Attack::deactivateRecovery()
{
	recovery_ = false;
	waitingForRecovery_ = false;
}

void Attack::attack()
{
	SoundEffect* s = &sdlutils().soundEffects().at("sfx_chica_attack2");
	s->play();
	newAttack_ = true;

	setActive(true);
	lastAttack = sdlutils().currRealTime();
	setPosition();//si no setamos la posicion aqui, se renderizara un frame del ataque en una posicion que no debe
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

void Attack::activateRecoveryTimer()
{
	recoveryTimer_.reset();

	recovery_ = true;
	waitingForRecovery_ = true;
}

void Attack::recoverAnim()
{
	anim_->changeanim(&sdlutils().images().at("Attack1_Recovery"), 3, 2, 150, 5, "Attack1_Recovery");
	anim_->repeat(false);

	//desactiva recovery_ al terminar
	std::function<void()> endCallback = [this]() { deactivateRecovery(); };
	anim_->registerEvent(std::pair<int, std::string>(4, "Attack1_Recovery"), endCallback);
}
