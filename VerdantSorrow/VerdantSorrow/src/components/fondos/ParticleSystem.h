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
	void createParticlesAsh(int numpart);

	void createParticlesDandellion(int numpart);

	void createOverlayParticlesDandellion(int numpart);

	void createBackgroundParticlesSymbols(int numpart);

	void createParticlesRain(int numpart);

	void createParticlesWind(int numpart);

	void createParticlesRoll(int numpart, int dir, int x, int y);

	void createParticlesSmash(int numpart, int x, int y);

	void createParticlesWave(int numpart, int x, int y);

	void createParticlesFireball(int numpart, int x, int y);

	void createParticlesFire(int numpart, int x, int y);

	void createParticlesRoots(int numpart, int x, int y);

	void createParticlesRootsDie(int numpart, int x);

	void createParticlesLanternMove(int numpart, int x, int y);
	
	void createParticlesLanternDamage(int numpart, int x, int y);

	void createParticlesEssence(int numpart, int x, int y, Transform* target);

	void createParticlesHandMagic(int numpart, Transform* target);

private:
	Texture* tex_;
	std::vector<ecs::Entity*>parts;
	ecs::Manager* mngr_;
	
};



