#pragma once
#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include "../RectangleCollider.h"
#include "../../game/CollisionManager.h"
#include "../FramedImage.h"
#include "PlayerAttributes.h"
#include <SDL.h>
#include <functional>
#include "../../sdlutils/VirtualTimer.h"

#include <vector>
using namespace std;

class Transform;
using ecs::Entity;

enum State { WAITING, ATTACKING, WAITING_RECOVERY, COOLDOWN };

class Attack : public RectangleCollider
{
public:
	__CMPID_DECL__(ecs::_ATTACK)
	Attack(float width, float height, float offsetY, CollisionManager* colManager);
	//Attack(float width, float height, CollisionManager* colManager, FramedImage* anim);
	~Attack();

	void initComponent() override;
	void update() override;

	void checkInput();

	void attackAir(std::function<void()>& attackCallback);

	void changeComboAnim(int& nCombo, int rows, int columns, int animDuration, int nFrames, int attackFrame, std::function<void()>& attackCallback, int recoveryFrame, std::function<void()>& recoveryCallback, VirtualTimer* comboTimer, bool ground);

	void attackGround(std::function<void()>& attackCallback);

	bool hasFinished();
	void setFinished(bool set);
	bool isNewAttack();
	void setNewAttack(bool set);

	bool hasFinishedRecovery();
	void deactivateRecovery();

	bool isAttacking();

    void attack();
    
protected:

	State state_;

	const int TIME_UNTIL_RECOVERY = 200;

	const int COMBO_WINDOW = 2000;

	const int MAX_COMBO = 3;

	Transform* tr_;

	//Variables que controlan el timing del ataque
	int attackDuration;
	int attackCoolDown;
	VirtualTimer* cooldownTimer_;
	VirtualTimer* attackTimer_;
	VirtualTimer* comboTimerGround_;
	VirtualTimer* comboTimerAir_;

	//combo
	bool comboFinishedGround_;
	bool comboFinishedAir_;
	int nComboGround_;
	int nComboAir_;

	bool newAttack_;
	bool finished_;

	CollisionManager* colMan_;
	FramedImage* anim_;
	PlayerAttributes* attrib_;

	vector<SDL_Scancode> attackKeys;
	vector<SDL_GameControllerButton> attackButtons;

	VirtualTimer* recoveryTimer_;
	bool recovery_;

	/**
	* Setea la posicion del ataque delante del jugador, teniendo en cuenta su direccion de movimiento
	*/
	void setPosition() override;

	void activateRecoveryTimer();

	void recoverAnim();
};