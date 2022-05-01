#include "OnBorders.h"

#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"

OnBorders::OnBorders(): tr_(nullptr)
{
}

OnBorders::~OnBorders()
{
}

void OnBorders::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void OnBorders::update()
{
	if (active_) {

		auto& pos = tr_->getPos();

		auto height = sdlutils().height();
		auto width = sdlutils().width();
		auto entHeight = tr_->getHeight();
		auto entWidth = tr_->getWidth();

		//Borde inferior
		if (pos.getY() > height - entHeight - GROUND_HEIGHT) {

			onBottom();
		}

		//Borde superior
		else if (pos.getY() < 0) {

			onTop();
		}

		//si no toca bordes superior e inferior
		else {

			onNoTopAndBot();
		}

		//Borde derecho
		if (pos.getX() > width - entWidth) {

			onRight();
		}
		//Borde izquierdo
		else if (pos.getX() < 0) {

			onLeft();
		}

		//si no toca bordes laterales
		else {

			onNoLeftAndRight();
		}
	}
}
