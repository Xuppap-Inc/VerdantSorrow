#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
#include "../../../sdlutils/VirtualTimer.h"

using ecs::Manager;
class Entity;
class Tranform;
class CollisionManager;
class Punietazo;
class ClapAttack;
class HammerArm;
class BossAtributos;
class RectangleCollider;
class Image;

class HandsManager :
	public ecs::Component
{
public:

	enum State {
		REPOSO = 0,
		CLAP,
		PUNIETAZO,
		MARTILLAZO
	};

	__CMPID_DECL__(ecs::_HANDS_MANAGER)
		HandsManager(CollisionManager* col_);
	~HandsManager();
	void initComponent()override;
	void update();

	State getState() { return state_; };
	void setState(State state) { state_ = state; };

	
private:
	void createHands();
	void chooseAttack();
	void clapAttack();
	void punietazoAttack();
	void hammerAttack();
	float lastAttackDone = 0, attackCooldown;
	int numeroAtaque;

	int multFase_;
	VirtualTimer tiempoColor_;

	ecs::Entity* leftHand_;
	ecs::Entity* rightHand_;
	Transform* leftHandTr_;
	Transform* rightHandTr_;
	Transform* tr_;
	Transform* playertr_;
	CollisionManager* colmanager_;
	Punietazo* punietazoright_;
	Punietazo* punietazoleft_;
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

