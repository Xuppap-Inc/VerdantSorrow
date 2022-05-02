#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"
#include<vector>

class ecs::Manager;
class Transform;

class ParticleSystem 
{
public:

	ParticleSystem(Texture* tex, ecs::Manager* mngr);
	void disolveParticles();
	
	void targetParticles(Transform* target);

	void createParticlesAsh(int numpart);

	void createParticlesMenu(int numpart);

	void createParticlesHub(int numpart);

	void createParticlesMenu2(int numpart);

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

	void createParticlesHandMagic(int numpart, Transform* target);

	void createParticlesBossDie(int numpart, int x, int y);

	void createParticlesBossDieSymbol(int numpart, int x, int y);

	void createParticlesBossSign(int numpart);

	void createParticlesBossSpawn(int numpart, int x, int y);

	void createParticlesDamagePlayer(int numpart, int x, int y);

	void createParticlesKillPlayer(int numpart, int x, int y);

private:

	Texture* tex_;
	std::vector<ecs::Entity*>parts;
	ecs::Manager* mngr_;
};



