
#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../../ecs/Entity.h"

class VirtualTimer;
class Image;
class HubAreas : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_NPC_CTRL)
		HubAreas(CollisionManager* colManager, std::string area);
	~HubAreas() {};

	void initComponent() override;
	void update() override;
	void render();


protected:
	Entity* dialogBox_;
	Transform* tr_;
	CollisionManager* colMan_;
	RectangleCollider* col_;
	VirtualTimer* vt_;
	int areatimer;
	bool showarea;
	std::string dialog_;
	float initime;
	std::string font_;
	bool collision;
	std::string text_;

};

