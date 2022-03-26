#pragma once
#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include "../RectangleCollider.h"
#include "../../game/CollisionManager.h"
#include "../FramedImage.h"
#include "PlayerAttributes.h"
#include <SDL.h>
#include <functional>

#include <vector>
using namespace std;

class Transform;
using ecs::Entity;

class Attack : public RectangleCollider
{
public:
	__CMPID_DECL__(ecs::_ATTACK)
		Attack(float width, float height, CollisionManager* colManager);
	//Attack(float width, float height, CollisionManager* colManager, FramedImage* anim);
	~Attack();

	void initComponent() override;
	void update() override;

	bool hasFinished();
	void setFinished(bool set);
	bool isNewAttack();
	void setNewAttack(bool set);

    void attack();
    
protected:
	Transform* tr_;

	//Variables que controlan el timing del ataque
	int attackDuration;
	int attackCoolDown;
	int lastAttack;

	bool newAttack_;
	bool finished_;

	CollisionManager* colMan_;
	FramedImage* anim_;
	PlayerAttributes* attrib_;

	vector<SDL_Scancode> attackKeys;
	vector<SDL_GameControllerButton> attackButtons;

	/**
	* Setea la posicion del ataque delante del jugador, teniendo en cuenta su direccion de movimiento
	*/
	void setPosition() override;


};

