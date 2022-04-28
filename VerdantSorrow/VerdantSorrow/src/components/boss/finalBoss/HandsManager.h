#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
#include "../../../sdlutils/VirtualTimer.h"

using ecs::Manager;
class Entity;
class Tranform;
class CollisionManager;
class Punch;
class ClapAttack;
class HammerArm;
class BossAtributos;
class RectangleCollider;
class Image;
class VirtualTimer;

class HandsManager :
	public ecs::Component
{
public:

	enum State {
		REPOSO = 0,
		CLAP,
		PUNIETAZO,
		MARTILLAZO,
		START_ANIM
	};

	__CMPID_DECL__(ecs::_HANDS_MANAGER)
		HandsManager(CollisionManager* col_);
	~HandsManager();
	void initComponent()override;
	void update();

	void checkPhaseChange();

	State getState() { return state_; };
	void setState(State state) { state_ = state; };

	
private:

	const int START_DELAY = 5000;

	void createHands();
	void chooseAttack();
	void clapAttack();
	void punietazoAttack();
	void hammerAttack();
	VirtualTimer* lastAttackDone_;
	float  attackCooldown;
	int numeroAtaque;

	int multFase_;
	VirtualTimer* tiempoColor_;

	VirtualTimer* startTimer_;

	ecs::Entity* leftHand_;
	ecs::Entity* rightHand_;
	Transform* leftHandTr_;
	Transform* rightHandTr_;
	Transform* tr_;
	Transform* playertr_;
	CollisionManager* colmanager_;
	Punch* punietazoright_;
	Punch* punietazoleft_;
	ClapAttack* clapLeft_;
	ClapAttack* clapRight_;
	HammerArm* hammerRight_;
	HammerArm* hammerLeft_;
	BossAtributos* bA_;
	RectangleCollider* colliderRightHand_;
	RectangleCollider* colliderLeftHand_;

	Image* leftHandImg_;
	Image* rightHandImg_;

	State state_;
};

