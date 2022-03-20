#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
using ecs::Manager;
class Entity;
class Tranform;
class CollisionManager;
class Punietazo;
class ClapAttack;
class HammerArm;
class BossAtributos;
class RectangleCollider;

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
	ecs::Entity* leftHand_;
	ecs::Entity* rightHand_;
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

	State state_;
};

