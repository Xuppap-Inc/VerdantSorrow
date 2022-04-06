// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class Capas;
class Transform;

class RenderParallax : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_PARALLAX)

		RenderParallax(Texture* tex,float scrollratio);
	virtual ~RenderParallax();
	void initComponent() override;
	void render()override;


private:

	Texture* tex_;
	Transform* tr_;
	float scrollratio_;


};

