// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;

class Transform;

class ShowAtOpposideSide : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_SHOWATOPPOSIDESIDE)

		ShowAtOpposideSide(bool camera = false);
	virtual ~ShowAtOpposideSide();
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;
	ecs::Entity* camera;
};

