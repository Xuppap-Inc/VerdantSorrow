#include "Attack.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"
#include "../../ecs/Manager.h"
#include "../VFX.h"


Attack::Attack(float width, float height, float offsetY, CollisionManager* colManager) :
	tr_(nullptr), RectangleCollider(width, height, 0, offsetY), attackDuration(200),
	attackCoolDown(300), newAttack_(false), finished_(true), recoveryTimer_(), 
	recovery_(false), cooldownTimer_(), comboFinishedGround_(false), attackTimer_(), 
	anim_(), attrib_(), nComboGround_(0), comboTimerGround_(), nComboAir_(0), comboFinishedAir_(false), comboTimerAir_(),

	// INPUT
	attackKeys({ SDL_SCANCODE_J }),
	attackButtons({ SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER })
{
	setActive(false);
	colMan_ = colManager;

	state_ = WAITING;
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

	attackTimer_.reset();
}

void Attack::update()
{
	if (state_ == WAITING || state_ == WAITING_RECOVERY) { //comprueba si el jugador está dentro de la ventana dada para hacer el combo
	
		checkInput();

		if (state_ == WAITING_RECOVERY) {

			if (recoveryTimer_.currTime() >= TIME_UNTIL_RECOVERY) {

				recoverAnim();

				//activa el cooldown
				cooldownTimer_.reset();

				state_ = COOLDOWN;
			}
		}

		else if (state_ == WAITING) {
		
			if (recovery_) deactivateRecovery();

			if (comboFinishedGround_ || comboTimerGround_.currTime() > COMBO_WINDOW) {
			
				//vars combo
				nComboGround_ = 0;
				comboFinishedGround_ = false;
			}

			if (attrib_->isOnGround() || comboFinishedAir_ || comboTimerAir_.currTime() > COMBO_WINDOW) {

				//vars combo
				nComboAir_ = 0;
				comboFinishedAir_ = false;
			}
		}
	}

	else if (state_ == COOLDOWN) {
	
		if (!comboFinishedGround_ && comboTimerGround_.currTime() < COMBO_WINDOW) {
		
			checkInput();
		}

		if (cooldownTimer_.currTime() >= attackCoolDown) {
		
			state_ = WAITING;
		}
	}

	if (!finished_) {
	
		setPosition();

		if (attackTimer_.currTime() >= attackDuration) {

			setActive(false);
			finished_ = true;

			if (state_ == ATTACKING) state_ = COOLDOWN;
		}
	}
}

void Attack::checkInput()
{
	bool isRolling = mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerCtrl>()->isRolling();

	if (!isRolling) {

		auto& ihdlr = ih();

		if (ihdlr.keyDownEvent() || ihdlr.controllerDownEvent()) {//si no esta activo, comprueba si se puede activar (cooldown y j presionada)

			bool attackButtonPressed = false;

			// Keyboard
			int i = 0;
			while (i < attackKeys.size() && !ihdlr.isKeyDown(attackKeys[i])) i++;
			if (i < attackKeys.size()) attackButtonPressed = true;
			// Controller
			i = 0;
			while (i < attackButtons.size() && !ihdlr.isControllerButtonDown(attackButtons[i])) i++;
			if (i < attackButtons.size()) attackButtonPressed = true;

			if (attackButtonPressed && finished_) {

				//callback que llama a attack
				std::function<void()> attackCallback = [this]() { attack(); };

				if (attrib_->isOnGround() && !comboFinishedGround_) {

					attackGround(attackCallback);
				}
				else if (!attrib_->isOnGround() && !comboFinishedAir_) {

					attackAir(attackCallback);
				}
			}
		}
	}
}

void Attack::attackAir(std::function<void()>& attackCallback)
{
	state_ = ATTACKING;

	anim_->repeat(false);

	if (nComboAir_ == 0) {

		anim_->changeanim(&sdlutils().images().at("Chica_AtkAir1"), 3, 3, 100, 8, "Chica_AtkAir1");

		//registra el evento en la animacion
		anim_->registerEvent(std::pair<int, std::string>(4, "Chica_AtkAir1"), attackCallback);

		nComboAir_++;

		comboTimerAir_.reset();
	}

	else if (nComboAir_ == 1) {

		anim_->changeanim(&sdlutils().images().at("Chica_AtkAir2"), 3, 3, 100, 9, "Chica_AtkAir2");

		//registra el evento en la animacion
		anim_->registerEvent(std::pair<int, std::string>(4, "Chica_AtkAir2"), attackCallback);

		nComboAir_++;

		comboTimerAir_.reset();
	}

	else if (nComboAir_ == 2) {

		anim_->changeanim(&sdlutils().images().at("Chica_AtkAir3"), 2, 6, 300, 11, "Chica_AtkAir3");

		//registra el evento en la animacion
		anim_->registerEvent(std::pair<int, std::string>(4, "Chica_AtkAir3"), attackCallback);

		comboFinishedAir_ = true;

		nComboAir_ = 0;
	}

	else {

		comboFinishedAir_ = false;

		nComboAir_ = 0;
	}
}

void Attack::attackGround(std::function<void()>& attackCallback)
{
	state_ = ATTACKING;

	//callback recovery
	std::function<void()> recoveryCallback = [this]() { activateRecoveryTimer(); };

	anim_->repeat(false);

	if (nComboGround_ == 0) {

		anim_->changeanim(&sdlutils().images().at("Chica_AtkFloor"), 2, 5, 100, 9, "Chica_AtkFloor");

		//registra el evento en la animacion
		anim_->registerEvent(std::pair<int, std::string>(6, "Chica_AtkFloor"), attackCallback);

		anim_->registerEvent(std::pair<int, std::string>(8, "Chica_AtkFloor"), recoveryCallback);

		nComboGround_++;

		comboTimerGround_.reset();
	}

	else if (nComboGround_ == 1) {

		anim_->changeanim(&sdlutils().images().at("Chica_AtkFloor2"), 3, 2, 60, 6, "Chica_AtkFloor2");

		//registra el evento en la animacion
		anim_->registerEvent(std::pair<int, std::string>(1, "Chica_AtkFloor2"), attackCallback);

		anim_->registerEvent(std::pair<int, std::string>(5, "Chica_AtkFloor2"), recoveryCallback);

		nComboGround_++;

		comboTimerGround_.reset();
	}

	else if (nComboGround_ == 2) {

		anim_->changeanim(&sdlutils().images().at("Chica_AtkFloor3"), 2, 5, 100, 9, "Chica_AtkFloor3");

		//registra el evento en la animacion
		anim_->registerEvent(std::pair<int, std::string>(5, "Chica_AtkFloor3"), attackCallback);

		anim_->registerEvent(std::pair<int, std::string>(8, "Chica_AtkFloor3"), recoveryCallback);

		comboFinishedGround_ = true;

		nComboGround_ = 0;
	}

	else {
	
		comboFinishedGround_ = false;

		nComboGround_ = 0;
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
}

bool Attack::isAttacking()
{
	return state_ != WAITING && state_ != COOLDOWN;
}

void Attack::attack()
{
	SoundEffect* s = &sdlutils().soundEffects().at("sfx_chica_attack2");
	s->play();
	finished_ = false;
	newAttack_ = true;

	setActive(true);
	attackTimer_.reset();
	setPosition();//si no setamos la posicion aqui, se renderizara un frame del ataque en una posicion que no debe
}

void Attack::setPosition()
{
	int playerMovementDir = ent_->getComponent<PlayerCtrl>()->getMovementDir();

	Vector2D contPos = tr_->getPos();

	if (playerMovementDir >= 0)
		pos_ = Vector2D(contPos.getX() + tr_->getWidth() + offsetX_, contPos.getY() + offsetY_);
	else
		pos_ = Vector2D(contPos.getX() - width_ + offsetX_, contPos.getY() + offsetY_);
}

void Attack::activateRecoveryTimer()
{
	recoveryTimer_.reset();

	recovery_ = true;
	state_ = WAITING_RECOVERY;
}

void Attack::recoverAnim()
{
	anim_->changeanim(&sdlutils().images().at("Attack1_Recovery"), 3, 2, 150, 5, "Attack1_Recovery");
	anim_->repeat(false);

	//desactiva recovery_ al terminar
	std::function<void()> endCallback = [this]() { deactivateRecovery(); };
	anim_->registerEvent(std::pair<int, std::string>(4, "Attack1_Recovery"), endCallback);
}
