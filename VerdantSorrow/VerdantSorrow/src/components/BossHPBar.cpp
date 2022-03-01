#include "BossHPBar.h"

#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

BossHPBar::BossHPBar() :
	attrib_(),maxHp(),maxBarLength(),pos() {
}

BossHPBar::~BossHPBar() {
}

void BossHPBar::initComponent() {
	attrib_ = ent_->getComponent<BossAtributos>();
	assert(attrib_ != nullptr);

	maxHp = attrib_->getLife();
	maxBarLength = sdlutils().width() * 0.5f;
	pos = Vector2D((sdlutils().width() - maxBarLength) / 2, sdlutils().height() - 75);
}

void BossHPBar::render() {


	SDL_SetRenderDrawColor(sdlutils().renderer(),255,0,0,255);

	SDL_Rect rect = build_sdlrect(pos.getX(), pos.getY(), (attrib_->getLife() >= 0 ? (maxBarLength * (attrib_->getLife() /maxHp)): 0), 10);

	SDL_RenderFillRect(sdlutils().renderer(), &rect);

}
