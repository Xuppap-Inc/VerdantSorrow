#include "NpcCtrl.h"
#include "../RectangleCollider.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../player/PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "DialogBoxMngr.h"
#include "../player/PlayerHubControl.h"

NpcCtrl::NpcCtrl(CollisionManager* colManager, Entity* dialogBox) : colMan_(colManager), dialogBox_(dialogBox), canTalk(true)
{
}

void NpcCtrl::initComponent()
{
	col_ = ent_->getComponent<RectangleCollider>();
	assert(col_ != nullptr);
}

void NpcCtrl::update()
{

	if (canTalk/* && dialogBox_->isActive()*/) {
		auto& ihdlr = ih();

		if (col_->isActive() && colMan_->hasCollisions(col_)) {
			std::vector<RectangleCollider*> colliders = colMan_->getCollisions(col_);

			for (auto c : colliders) {
				if (c->isActive() && c->getEntity()->getComponent<PlayerCtrl>() != nullptr) {
					if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_E)) {
						mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerHubControl>()->changeStateTalk(true);
						dialogBox_->getComponent<DialogBoxMngr>()->activate("Al comenzar el juego, se le mostrará al jugador el grueso de la historia, el por qué está ahí y cual es el argumento que lo rodea. Decimos que se le cuenta el grueso pero no el total, puesto que a lo largo del juego a medida que hable con los NPCs del Hub estos, le informarán de manera progresiva de todo lo que ha pasado, qué fue lo que llevó al mundo de Arven a su estado actual, qué se espera de él y distintos detalles argumentales para dar sensación de una historia de la que el jugador es partícipe y pieza clave.");
						
						if (dialogBox_->getComponent<DialogBoxMngr>()->isFinished() && dialogBox_->getComponent<DialogBoxMngr>()->isLastParagraph()) {
							canTalk = false;
							mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerHubControl>()->changeStateTalk(false);
							dialogBox_->getComponent<DialogBoxMngr>()->desactivate();
						}
							
						else if (dialogBox_->getComponent<DialogBoxMngr>()->isFinished()) {
							dialogBox_->getComponent<DialogBoxMngr>()->changeFinishedState(false);
						}
							
						
					}
				}
			}
		}
	}
}
