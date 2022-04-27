#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
#include "../../../sdlutils/VirtualTimer.h"
using ecs::Manager;
class Entity;
class CollisionManager;
class Image;
class VirtualTimer;

class Punch :
	public ecs::Component
{
public:
	enum State {
		DOWN = 0, 
		FOLLOW,
		HIT, 
		BACK,
		REPOSO,
		FIN
	};
	__CMPID_DECL__(ecs::_PUNIETAZO)
		Punch(bool manoDerecha);
	~Punch();
	void initComponent()override;


	void goDown();
	void followPlayer();
	void hit();
	void goBack();

	State getstate() { return state_; };
	void changeState(State s) { state_ = s; };

private:
	Transform* tr_;
	CollisionManager* colmanager_;
	RectangleCollider* col_;
	Transform* playertr_;
	Image* img_;

	State state_;

	VirtualTimer* hitTime_;
	VirtualTimer* goBackTime_;

	//initial hand tr
	float initialwidth_;
	float initialheight_;

	// Initial hand collider
	float col_initialwidth_;
	float col_initialheight_;
	Vector2D initialpos_;
	float dW, dH;

	//max hand size
	float maxWidth_;
	float maxHeight_;

	//hand speed
	int handSpeed_;

	bool manoDerecha_;

};

