#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../../ecs/Entity.h"

class VirtualTimer;
class Image;
class NpcCtrl : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_NPC_CTRL)
	NpcCtrl(CollisionManager* colManager, Entity* dialogBox);
	~NpcCtrl() {};

	void initComponent() override;
	void update() override;
	void setDialog(std::string dialog) {
		dialog_ = dialog;
	};
	

protected:
	Entity* dialogBox_;
	Transform* tr_;
	CollisionManager* colMan_;
	RectangleCollider* col_;
	VirtualTimer* vt_;
	int dialogTimer;
	bool canTalk;
	std::string dialog_;
	Image* letterE;

	SDL_GameControllerButton x_Button;
	SDL_GameControllerButton a_Button;
};

