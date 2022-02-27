
#pragma once
#include "../ecs/Component.h"
#include "FrogBoss/BossAtributos.h"
#include "../utils/Vector2D.h"
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

	//maxima vida del boss
	float maxHp;
	//maximo width de la barra
	float maxBarLength;
	//posicion de la barra
	Vector2D pos;
};

