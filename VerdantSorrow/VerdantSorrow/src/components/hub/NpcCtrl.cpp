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

						//Activa el diálogo si está desactivado e inmoviliza al jugador
						if (!dialogBox_->isActive()) {
							mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerHubControl>()->changeStateTalk(true);
							dialogMngr->activate(sdlutils().dialogs().at("npc4_dialogue4"));
						}

						//Si ha terminado y es el último párrafo se desactiva
						else if (dialogMngr->isFinished() && dialogMngr->isLastParagraph()) {
							canTalk = false;
							mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerHubControl>()->changeStateTalk(false);
							dialogMngr->desactivate();
							vt_->reset();
						}

						//Si ha terminado y no es el último párrafo vacía el vector para el siguiente párrafo
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
