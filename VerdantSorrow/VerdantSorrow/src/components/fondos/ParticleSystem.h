#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class Transform;

class ParticleSystem 
{
public:

	ParticleSystem(Texture* tex,ecs::Manager*mngr);
	void createParticlesSnow(int numpart);

	void createParticlesDirt(int numpart, int dir, int x, int y);

	void createParticlesEssence(int numpart, int x, int y, Transform* target);

private:
	Texture* tex_;
	std::vector<ecs::Entity*>parts;
	ecs::Manager* mngr_;
	
};



