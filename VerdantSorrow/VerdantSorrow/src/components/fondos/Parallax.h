// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class Capas;

class Parallax : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_PARALLAX)

		Parallax();
	virtual ~Parallax();
	void initComponent() override;
	void newLayer(Texture* tex, float vel);
	void update() override;
private:
	std::vector<Capas*>layer;
};

