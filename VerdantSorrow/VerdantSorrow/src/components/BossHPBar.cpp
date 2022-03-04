#include "BossHPBar.h"

#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

BossHPBar::BossHPBar() :
	attrib_(), maxHp(), maxBarLength(), pos(), accumulatedDamage(), lastHP(), accumulatedDmgDecrease(), accumulatedDmgFirstDecrease() {
}

BossHPBar::~BossHPBar() {
}

void BossHPBar::initComponent() {
	attrib_ = ent_->getComponent<BossAtributos>();
	assert(attrib_ != nullptr);

	maxHp = attrib_->getLife();
	lastHP = attrib_->getLife();
	maxBarLength = sdlutils().width() * 0.5f;
	pos = Vector2D((sdlutils().width() - maxBarLength) / 2, sdlutils().height() - 75);
}

void BossHPBar::render() {

	//suma de daño acumulado
	if (accumulatedDamage == 0)
		accumulatedDmgFirstDecrease = sdlutils().currRealTime() + 1000;

	accumulatedDamage += (lastHP - attrib_->getLife());

	//barra roja
	SDL_Rect rect = build_sdlrect(pos.getX(), pos.getY(), (attrib_->getLife() >= 0 ? (maxBarLength * (attrib_->getLife() / maxHp)) : 0), 10);
	//barra de daño acumulado
	SDL_Rect rect2 = build_sdlrect(pos.getX() + rect.w, pos.getY(), maxBarLength * (accumulatedDamage / maxHp), rect.h);


	//dibujar barras
	SDL_SetRenderDrawColor(sdlutils().renderer(), 58, 2, 0, 255);
	SDL_RenderFillRect(sdlutils().renderer(), &rect);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 110, 89, 18, 255);
	SDL_RenderFillRect(sdlutils().renderer(), &rect2);

	//cada accumulatedDmgDecreaseCooldown reduce un poco la barra de daño acumulado
	//si el daño acumulado era 0, espera un poco antes de empezar a bajar
	if (sdlutils().currRealTime() >= accumulatedDmgFirstDecrease) {
		if (sdlutils().currRealTime() >= accumulatedDmgDecrease) {
			accumulatedDamage = (accumulatedDamage - 0.05 > 0 ? accumulatedDamage -= 0.05 : accumulatedDamage = 0);
			accumulatedDmgDecrease = sdlutils().currRealTime() + 15;
		}
	}

	lastHP = attrib_->getLife();

}
