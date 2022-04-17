#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class ParticleSystem 
{
public:

	ParticleSystem(Texture* tex,ecs::Manager*mngr);
	void createParticlesSnow(int numpart);

private:
	Texture* tex_;
	std::vector<ecs::Entity*>parts;
	ecs::Manager* mngr_;
	
};



