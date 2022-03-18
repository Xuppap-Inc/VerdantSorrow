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

WaveSpawner::WaveSpawner(CollisionManager* colMngr) : colManager_(colMngr)
{
}

void WaveSpawner::createWaves()
{
	createWave(-1);
	createWave(1);
}

void WaveSpawner::createWave(int dir)
{
	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();

	//Se anyade el transform
	auto WaveTr = Wave->addComponent<Transform>();
	auto WaveX = playerTr_->getPos().getX();
	auto WaveY = sdlutils().height() - waveHeight;

	if (dir == 1)
		WaveX += playerTr_->getWidth();

	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la onda
	WaveTr->init(Vector2D(WaveX, WaveY), Vector2D(), waveWidth, waveHeight, 0.0f);
	//Se le anyade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se anyade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	colManager_->addCollider(waveCollider);

	//Se anyade el movimiento horizontal
	Wave->addComponent<WaveMovement>(dir, waveSpeed_);
}
