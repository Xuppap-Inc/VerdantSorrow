#include "Attack.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"
#include "../../ecs/Manager.h"


Attack::Attack(float width, float height, float offsetY, CollisionManager* colManager) :
	tr_(nullptr), RectangleCollider(width, height, 0, offsetY), attackDuration(200),
	attackCoolDown(300), newAttack_(false), finished_(true), recoveryTimer_(),
	recovery_(false), cooldownTimer_(), comboFinishedGround_(false), attackTimer_(),
	anim_(), attrib_(), nComboGround_(0), comboTimerGround_(), nComboAir_(0), comboFinishedAir_(false), comboTimerAir_(),

	// INPUT
	attackKeys({ SDL_SCANCODE_J,SDL_SCANCODE_K,SDL_SCANCODE_L }),
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
	attackTimer_ = mngr_->addTimer();
	recoveryTimer_ = mngr_->addTimer();
	comboTimerAir_ = mngr_->addTimer();
	comboTimerGround_ = mngr_->addTimer();
	cooldownTimer_ = mngr_->addTimer();
}

void Attack::update()
{
	if (state_ == WAITING || state_ == WAITING_RECOVERY) { //comprueba si el jugador está dentro de la ventana dada para hacer el combo

		checkInput();

		if (state_ == WAITING_RECOVERY) {

			if (recoveryTimer_->currTime() >= TIME_UNTIL_RECOVERY) {

				recoverAnim();

				//activa el cooldown
				cooldownTimer_->reset();

				state_ = COOLDOWN;
			}
		}

		else if (state_ == WAITING) {

			if (recovery_) deactivateRecovery();

			if (comboFinishedGround_ || comboTimerGround_->currTime() > COMBO_WINDOW) {

				//vars combo
				nComboGround_ = 0;
				comboFinishedGround_ = false;
			}

			if (attrib_->isOnGround() || comboFinishedAir_ || comboTimerAir_->currTime() > COMBO_WINDOW) {

				//vars combo
				nComboAir_ = 0;
				comboFinishedAir_ = false;
			}
		}
	}

	else if (state_ == COOLDOWN) {

		if (!comboFinishedGround_ && comboTimerGround_->currTime() < COMBO_WINDOW) {

			checkInput();
		}

		if (cooldownTimer_->currTime() >= attackCoolDown) {

			state_ = WAITING;
		}
	}

	if (!finished_) {

		setPosition();

		if (attackTimer_->currTime() >= attackDuration) {

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

	std::function<void()> recoveryCallback = []() {};

	if (nComboAir_ == 0) {

		int rows = 3;
		int columns = 3;
		int animDuration = 100;
		int nFrames = 8;
		int attackFrame = 4;
		int recoveryFrame = -1;

		changeComboAnim(nComboAir_, rows, columns, animDuration, nFrames, attackFrame, attackCallback, recoveryFrame, recoveryCallback, comboTimerAir_, false);
	}

	else if (nComboAir_ == 1) {

		int rows = 3;
		int columns = 3;
		int animDuration = 100;
		int nFrames = 9;
		int attackFrame = 4;
		int recoveryFrame = -1;

		changeComboAnim(nComboAir_, rows, columns, animDuration, nFrames, attackFrame, attackCallback, recoveryFrame, recoveryCallback, comboTimerAir_, false);
	}

	else if (nComboAir_ == 2) {

		int rows = 2;
		int columns = 6;
		int animDuration = 300;
		int nFrames = 11;
		int attackFrame = 4;
		int recoveryFrame = -1;

		changeComboAnim(nComboAir_, rows, columns, animDuration, nFrames, attackFrame, attackCallback, recoveryFrame, recoveryCallback, comboTimerAir_, false);

		comboFinishedAir_ = true;

		nComboAir_ = 0;
	}

	else {

		comboFinishedAir_ = false;

		nComboAir_ = 0;
	}
}

void Attack::changeComboAnim(int& nCombo, int rows, int columns, int animDuration, int nFrames, int attackFrame, std::function<void()>& attackCallback, int recoveryFrame, std::function<void()>& recoveryCallback, VirtualTimer* comboTimer, bool ground)
{
	std::string animName;

	int n = nCombo + 1;
	if (ground) animName = "Chica_AtkFloor" + to_string(n);
	else animName = "Chica_AtkAir" + to_string(n);

	anim_->changeanim(&sdlutils().images().at(animName), rows, columns, animDuration, nFrames, animName);

	//registra el evento en la animacion
	anim_->registerEvent(std::pair<int, std::string>(attackFrame, animName), attackCallback);
	anim_->registerEvent(std::pair<int, std::string>(attackFrame, animName), recoveryCallback);

	nCombo++;

	comboTimer->reset();
}

void Attack::attackGround(std::function<void()>& attackCallback)
{
	state_ = ATTACKING;

	//callback recovery
	std::function<void()> recoveryCallback = [this]() { activateRecoveryTimer(); };

	anim_->repeat(false);

	if (nComboGround_ == 0) {

		int rows = 2;
		int columns = 5;
		int animDuration = 100;
		int nFrames = 9;
		int attackFrame = 6;
		int recoveryFrame = 8;

		changeComboAnim(nComboGround_, rows, columns, animDuration, nFrames, attackFrame, attackCallback, recoveryFrame, recoveryCallback, comboTimerGround_, true);
	}

	else if (nComboGround_ == 1) {

		int rows = 3;
		int columns = 2;
		int animDuration = 60;
		int nFrames = 6;
		int attackFrame = 1;
		int recoveryFrame = 5;

		changeComboAnim(nComboGround_, rows, columns, animDuration, nFrames, attackFrame, attackCallback, recoveryFrame, recoveryCallback, comboTimerGround_, true);
	}

	else if (nComboGround_ == 2) {

		int rows = 2;
		int columns = 5;
		int animDuration = 100;
		int nFrames = 9;
		int attackFrame = 5;
		int recoveryFrame = 8;

		changeComboAnim(nComboGround_, rows, columns, animDuration, nFrames, attackFrame, attackCallback, recoveryFrame, recoveryCallback, comboTimerGround_, true);

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
	attackTimer_->reset();
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
	recoveryTimer_->reset();

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
