#include "NpcCtrl.h"
#include "../RectangleCollider.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../player/PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "DialogBoxMngr.h"
#include "../player/PlayerHubControl.h"

NpcCtrl::NpcCtrl(CollisionManager* colManager, Entity* dialogBox) : colMan_(colManager), dialogBox_(dialogBox), canTalk(true), dialogTimer(200)
{
}

void NpcCtrl::initComponent()
{
	col_ = ent_->getComponent<RectangleCollider>();
	assert(col_ != nullptr);
	vt_ = new VirtualTimer();
}

void NpcCtrl::update()
{
	if (vt_->currTime() > dialogTimer)
		canTalk = true;

	if (canTalk) {

		auto& ihdlr = ih();

		if (col_->isActive() && colMan_->hasCollisions(col_)) {

			DialogBoxMngr* dialogMngr = dialogBox_->getComponent<DialogBoxMngr>();

			for (auto c : colMan_->getCollisions(col_)) {
				if (c->isActive() && c->getEntity()->getComponent<PlayerCtrl>() != nullptr) {
					if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_E)) {

						//Activa el di�logo si est� desactivado e inmoviliza al jugador
						if (!dialogBox_->isActive()) {
							mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerHubControl>()->changeStateTalk(true);
							dialogMngr->activate("Al comenzar el juego, se le mostrar� al jugador el grueso de la historia, el por qu� est� ah� y cual es el argumento que lo rodea. Decimos que se le cuenta el grueso pero no el total, puesto que a lo largo del juego a medida que hable con los NPCs del Hub estos, le informar�n de manera progresiva de todo lo que ha pasado, qu� fue lo que llev� al mundo de Arven a su estado actual, qu� se espera de �l y distintos detalles argumentales para dar sensaci�n de una historia de la que el jugador es part�cipe y pieza clave.");
						}

						//Si ha terminado y es el �ltimo p�rrafo se desactiva
						else if (dialogMngr->isFinished() && dialogMngr->isLastParagraph()) {
							canTalk = false;
							mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerHubControl>()->changeStateTalk(false);
							dialogMngr->desactivate();
							vt_->reset();
						}

						//Si ha terminado y no es el �ltimo p�rrafo vac�a el vector para el siguiente p�rrafo
						else if (dialogMngr->isFinished()) {
							dialogMngr->changeFinishedState();
						}

						else {
							if (dialogMngr->isQuick())
								dialogMngr->changeSkip(true);
							else
								dialogMngr->changeTextSpeed(true);
						}
					}
				}
			}
		}
	}
}
