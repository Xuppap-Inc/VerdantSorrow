#include "WaveSpawner.h"

#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "WaveMovement.h"


#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Component.h"

#include <cassert>

WaveSpawner::WaveSpawner(CollisionManager* colMngr) : colManager_(colMngr), waveSpeed_(5), waveHeight_(50), waveWidth_(150)
{
}

void WaveSpawner::createWaves(Transform* tr)
{
	createWave(-1, tr);
	createWave(1, tr);
}

void WaveSpawner::createWave(int dir, Transform* tr)
{
	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();


	float WaveY = sdlutils().height() - waveHeight_, WaveX;

	if (dir > 0)
		WaveX = tr->getPos().getX() + tr->getWidth();
	else
		WaveX = tr->getPos().getX() - waveWidth_;

	//Se anyade el transform
	auto WaveTr = Wave->addComponent<Transform>(Vector2D(WaveX, WaveY), Vector2D(), waveWidth_, waveHeight_, 0.0f);

	//Se le anyade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se anyade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	colManager_->addCollider(waveCollider);

	//Se anyade el movimiento horizontal
	Wave->addComponent<WaveMovement>(dir, waveSpeed_);
}
