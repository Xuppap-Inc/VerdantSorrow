#include "PlayerDeathZone.h"
#include "Mushroom.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"

#include "../../player/PlayerAttributes.h"
#include "../../../game/SceneManager.h"
#include "../../../game/EscapeScene.h"
#include "../../../game/Hub.h"
#include "../../../game/Game.h"

#include <iostream>
#include <fstream>

PlayerDeathZone::PlayerDeathZone(CollisionManager* colManager, std::string s) : tr_(nullptr), colMan_(colManager), collider_(nullptr), area_(s)
{
}

PlayerDeathZone::~PlayerDeathZone()
{
}

void PlayerDeathZone::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();

	assert(tr_ != nullptr && collider_ != nullptr);
	collider_->setIsTrigger(true);
}

void PlayerDeathZone::update()
{
	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		for (auto c : colMan_->getCollisions(collider_)) {
			if (c->isActive() && !c->isTrigger() && c->getEntity()->getComponent<PlayerAttributes>()) {
				if (area_ == "died")
					sC().getEscapeScene()->init();
				else {
					Game::instance()->state_ = Game::State::FINALDEFEATED;

					std::ofstream myfile("resources/config/guardado.txt");
					if (myfile.is_open())
					{
						myfile << Game::State::FINALDEFEATED;
						myfile.close();
					}
					else std::cout << "No se puede abrir el guardado.txt";
					sC().changeScene(SceneManager::Hub_);
				}
			}
		}
	}
}


