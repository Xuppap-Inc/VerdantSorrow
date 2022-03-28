#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>
class Entity;
class Transform;

class ParticleSystem : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PARTICLE_SYSTEM);
	ParticleSystem(Texture* tex, int numpart);
	void initComponent() override;
	void update() override;
	void render() override;
private:
	int numpart_;
	Texture* tex_;
	std::vector<ecs::Entity*>parts;
	
};



