#include "HubAreas.h"
#include "../RectangleCollider.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../player/PlayerCtrl.h"
#include "DialogBoxMngr.h"
#include "../player/PlayerHubControl.h"
#include "../Transform.h"
#include "../Image.h"

HubAreas::HubAreas(CollisionManager* colManager,string area) : colMan_(colManager), showarea(true), areatimer(3000), dialog_(), tr_(nullptr), initime(-1),font_("PTMONO24"),collision(false)
{ 
	if (area == "eye") {
		text_ = "etheria";
	}
	else if (area == "frog") {
		text_ = "terria";
	}
	else if (area == "tree") {
		text_ = "lumine";
	}
	
}

void HubAreas::initComponent()
{
	col_ = ent_->getComponent<RectangleCollider>();
	tr_ = ent_->getComponent<Transform>();
	vt_ = mngr_->addTimer();
	assert(col_ != nullptr, tr_ != nullptr);
	image_ = ent_->addComponent<Image>(&sdlutils().imagesHub().at("etheria"));
	image_->setVisible(false);
}

void HubAreas::update()
{


	bool h=false;
		if (col_->isActive() && colMan_->hasCollisions(col_)) {
			for (auto c : colMan_->getCollisions(col_)) {
				if (c->isActive() && c->getEntity()->getComponent<PlayerCtrl>() != nullptr) {
					if (initime < 0) {
						initime = sdlutils().currRealTime();
					}					
					collision = true;
					h = true;
				}
				
			}

		}
		if (!h)initime = -1;
		
	
	
}

void HubAreas::render()
{


	if(collision){
		if (!(sdlutils().currRealTime() - initime >= areatimer)) {

			Texture* text=&sdlutils().imagesHub().at(text_);
			SDL_Rect dest = build_sdlrect(sdlutils().width() / 2 -(text->width()/2), sdlutils().height() - text->height(), text->width() , text->height() );
			auto sW = mngr_->getWindowScaleWidth();
			auto sH = mngr_->getWindowScaleHeight();
			//escalado pantalla
			dest.x *= sW;
			dest.w *= sW;
			dest.y *= sH;
			dest.h *= sH;
			text->render(dest);
		}
		
			
	}

}


