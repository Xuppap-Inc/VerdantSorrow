#pragma once

#include "../../../ecs/Component.h"

class CollisionManager;
class Transform;

class WaveSpawner: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_WAVE_SPAWNER)
	WaveSpawner(CollisionManager* colMngr);
	~WaveSpawner() {};

	//crea waves hacia los dos lados
	void createWaves(Transform* tr);

private:
	//crea una wave hacia la direccion dir
	void createWave(int dir, Transform* tr);

	CollisionManager* colManager_;

	float waveSpeed_;
	float waveHeight_;
	float waveWidth_;
};