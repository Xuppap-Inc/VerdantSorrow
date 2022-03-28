#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class ParticleSystem 
{
public:

	ParticleSystem(Texture* tex, int numpart,ecs::Manager*mngr);
	void createParticles();

private:
	int numpart_;
	Texture* tex_;
	std::vector<ecs::Entity*>parts;
	ecs::Manager* mngr_;
	
};



