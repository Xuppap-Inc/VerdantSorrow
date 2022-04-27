#include "NpcCtrl.h"
#include "../RectangleCollider.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../player/PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "DialogBoxMngr.h"
#include "../player/PlayerHubControl.h"
#include "../Transform.h"
#include "../Image.h"

NpcCtrl::NpcCtrl(CollisionManager* colManager, Entity* dialogBox) : colMan_(colManager), dialogBox_(dialogBox), canTalk(true), dialogTimer(200),dialog_(),tr_(nullptr),firstcol_(false)
{
}

void NpcCtrl::initComponent()
{
	col_ = ent_->getComponent<RectangleCollider>();
	tr_ = ent_->getComponent<Transform>();
	assert(col_ != nullptr,tr_!=nullptr);
	vt_ = mngr_->addTimer();
	auto letter = mngr_->addEntity();
	auto pos = tr_->getPos();
	letter->addComponent<Transform>( Vector2D(pos.getX()+tr_->getWidth()/2,pos.getY() + tr_->getHeight()/1.5) , Vector2D(), 30, 30, 0);
	letterE =letter->addComponent<Image>(&sdlutils().imagesHub().at("letterE"));
	letterE->setVisible(false);
	letter->addToGroup(ecs::_FIRST_GRP);
}

void NpcCtrl::update()
{
	bool letter = false;
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
							dialogMngr->activate(dialog_);
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
					else letter = true;
						
					
				}
			}
			
		}
	}
	letterE->setVisible(letter);
}


