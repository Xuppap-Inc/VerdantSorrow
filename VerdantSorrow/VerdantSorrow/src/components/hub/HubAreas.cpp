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
		text_ = "Cherry Blossom Forest";
	}
	else if (area == "frog") {
		text_ = "Frog Swamp";
	}
	else if (area == "tree") {
		text_ = "Fireflies Forest";
	}
	else text_ = "Rest zone";
}

void HubAreas::initComponent()
{
	col_ = ent_->getComponent<RectangleCollider>();
	tr_ = ent_->getComponent<Transform>();
	vt_ = mngr_->addTimer();
	assert(col_ != nullptr, tr_ != nullptr);
	
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

		Texture text(sdlutils().renderer(), text_, sdlutils().fontsHub().at(font_), build_sdlcolor(0xffffffff));
		SDL_Rect dest = build_sdlrect(sdlutils().width()/2-(text.width()*2), sdlutils().height()-130, text.width()*4, text.height()*4 );
		auto sW = mngr_->getWindowScaleWidth();
		auto sH = mngr_->getWindowScaleHeight();
		//escalado pantalla
		dest.x *= sW;
		dest.w *= sW;
		dest.y *= sH;
		dest.h *= sH;
		text.render(dest);
		  //initime = sdlutils().currRealTime();
		}	
		

		
	}

}


