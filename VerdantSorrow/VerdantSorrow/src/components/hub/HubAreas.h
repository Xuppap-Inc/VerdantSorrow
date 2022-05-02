
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

	int areatimer;
	bool showarea;
	
	float initime;

	bool collision;
	std::string text_;
	Image* image_;
	int i = 0;
	int j = 0;
};

