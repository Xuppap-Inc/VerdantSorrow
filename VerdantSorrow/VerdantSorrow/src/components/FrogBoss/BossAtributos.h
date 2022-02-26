#pragma once
class BossAtributos :
	public ecs::Component
{
private:
	float life_;
public:
	__CMPID_DECL__(ecs::_BOSS_ATTRIBUTES)
	BossAtributos() :life_(){};
	BossAtributos(float vida) : life_(vida)	{};

	float getLife() {
		return life_;
	}

	void setDamage(float l) {
		life_ -= l;
	}
};

