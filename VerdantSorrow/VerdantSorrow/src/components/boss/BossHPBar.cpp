#include "BossHPBar.h"

#include <cassert>

#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include "../../game/SceneManager.h"

BossHPBar::BossHPBar() :
	attrib_(), maxBarLength(), pos(), accumulatedDamage(),
	lastHP(), accumulatedDmgDecrease(), accumulatedDmgFirstDecrease(),
	bossBarArrow_left(&sdlutils().images().at("bossBarArrow_Left")),
	bossBarArrow_right(&sdlutils().images().at("bossBarArrow_Right")),
	middleBar(&sdlutils().images().at("middleBar"))
{ }

BossHPBar::~BossHPBar() {
}

void BossHPBar::initComponent() {
	if(SceneManager::scenes::Frog_ == sC().getScene()) attrib_ = mngr_->getHandler(ecs::_FROGBOSS)->getComponent<BossAtributos>();
	else if (SceneManager::scenes::Tree_ == sC().getScene()) attrib_ = mngr_->getHandler(ecs::_LANTERN)->getComponent<BossAtributos>();
	else if (SceneManager::scenes::Eye_ == sC().getScene()) attrib_ = mngr_->getHandler(ecs::_EYE)->getComponent<BossAtributos>();
	assert(attrib_ != nullptr);

	lastHP = attrib_->getLife();
	maxBarLength = sdlutils().width() * 0.5f;
	pos = Vector2D((sdlutils().width() - maxBarLength) / 2, sdlutils().height() - 50);
}

void BossHPBar::render() {

	//suma de daño acumulado
	if (accumulatedDamage == 0)
		accumulatedDmgFirstDecrease = sdlutils().currRealTime() + 1000;

	accumulatedDamage += (lastHP - attrib_->getLife());

	float xOffset = 350;
	float xLeft = sdlutils().width() / 2 - xOffset - 5;
	float xRight = sdlutils().width() / 2 + xOffset - 45;

	// Fondo de las barras
	SDL_Rect left = build_sdlrect(xLeft, (int)pos.getY(), 50, 50);
	SDL_Rect right = build_sdlrect(xRight, (int)pos.getY(), 50, 50);
	SDL_Rect middle = build_sdlrect((xRight - xLeft) / 2 - 10, ((int)pos.getY()) + 12, 640, 25);

	//barra roja
	SDL_Rect rect = build_sdlrect(pos.getX(), pos.getY() + 18, maxBarLength * (attrib_->getLife() / attrib_->getMaxHp()), 10);
	//barra de daño acumulado
	SDL_Rect rect2 = build_sdlrect(pos.getX() + rect.w, pos.getY() + 18, maxBarLength * (accumulatedDamage / attrib_->getMaxHp()), rect.h);

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	scaleRect(left, sW, sH);
	scaleRect(right, sW, sH);
	scaleRect(middle, sW, sH);
	scaleRect(rect, sW, sH);
	scaleRect(rect2, sW, sH);

	//dibujar interfaz barra
	bossBarArrow_left->render(left); // 50 grosor
	bossBarArrow_right->render(right); // 50 grosor
	middleBar->render(middle); // 25 grosor

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

void BossHPBar::scaleRect(SDL_Rect& rect, float sW, float sH)
{
	rect.x *= sW;
	rect.w *= sW;
	rect.y *= sH;
	rect.h *= sH;
}
