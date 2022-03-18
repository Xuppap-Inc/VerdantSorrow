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
	void createWaves();

private:
	//crea una wave hacia la direccion dir
	void createWave(int dir);

	Transform* playerTr_;
	CollisionManager* colManager_;

	float waveSpeed_ = 5;
	float waveHeight = 50;
	float waveWidth = 150;
};