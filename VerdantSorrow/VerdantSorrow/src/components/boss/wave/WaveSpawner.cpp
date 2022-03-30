#include "WaveSpawner.h"

#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../Image.h"
#include "../../../game/CollisionManager.h"
#include "WaveMovement.h"


#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Component.h"

#include <cassert>
#include "../../FramedImage.h"


WaveSpawner::WaveSpawner(CollisionManager* colMngr) : colManager_(colMngr), waveSpeed_(4)
{ }

void WaveSpawner::createWaves(float width, float height, Vector2D dir, Transform* tr, Texture* tex)
{
	createWave(width, height, dir, tr, tex);
	createWave(width, height, Vector2D(-dir.getX(), -dir.getY()), tr, tex);
}

void WaveSpawner::createWave(float width, float height, Vector2D dir, Transform* tr, Texture* tex)
{
	auto wave = mngr_->addEntity();

	float waveX, waveY = tr->getPos().getY() + tr->getHeight() - height;
	if (dir.getX() > 0)
		waveX = tr->getPos().getX() + tr->getWidth();
	else
		waveX = tr->getPos().getX() - width;

	//auto tr_ = wave->addComponent<Transform>(Vector2D(waveX, waveY), Vector2D(), width, height, 0.0f);
	auto tr_ = wave->addComponent<Transform>();
	tr_->init(Vector2D(waveX, waveY), Vector2D(), width, height, 0.0f, 20.0f);

	auto waveimg = wave->addComponent<FramedImage>(& sdlutils().images().at("wave"),3,3,2000,9,"wave");
	//Se anyade un collider a la onda
	auto waveCollider = wave->addComponent<RectangleCollider>(width, height);
	waveCollider->setIsTrigger(true);
	colManager_->addCollider(waveCollider);

	//Se anyade el movimiento horizontal
	wave->addComponent<WaveMovement>(dir, waveSpeed_);
}


