#pragma once

#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"

class CollisionManager;
class Transform;
class Texture;

class WaveSpawner : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_WAVE_SPAWNER)
		WaveSpawner(CollisionManager* colMngr);
	~WaveSpawner() {};

	//crea waves hacia los dos lados
	void createWaves(float width, float height, Vector2D dir, Transform* tr, Texture* tex = nullptr);
	//crea una wave hacia la direccion dir
	void createWave(float width, float height, Vector2D dir, Transform* tr, Texture* tex = nullptr);
private:
	CollisionManager* colManager_;

	float waveSpeed_;
};