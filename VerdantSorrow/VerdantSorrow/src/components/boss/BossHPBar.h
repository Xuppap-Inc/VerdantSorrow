
#pragma once
#include "../../ecs/Component.h"
#include "BossAtributos.h"
#include "../../utils/Vector2D.h"
#include "../../sdlutils/Texture.h"
class BossHPBar : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_BOSS_HP_BAR)

		BossHPBar();
	virtual ~BossHPBar();

	void initComponent() override;
	void render() override;

private:
	//atributos del boss
	BossAtributos* attrib_;

	//maximo width de la barra
	float maxBarLength;
	//posicion de la barra
	Vector2D pos;

	//variables para que la barra de vida baje poco a poco
	float accumulatedDamage;
	float lastHP;
	int accumulatedDmgDecrease;
	int accumulatedDmgFirstDecrease;

	// Texturas
	Texture* bossBarArrow_left;
	Texture* bossBarArrow_right;
	Texture* middleBar;
};

