#pragma once
class BossAtributos :
	public ecs::Component
{
private:
	int life_;
	int maxHp_;
	bool onGround_;
	bool defeated_;
public:
	__CMPID_DECL__(ecs::_BOSS_ATTRIBUTES)
	BossAtributos(int vida) : life_(vida), maxHp_(vida), onGround_(false), defeated_(false) {};

	float getLife() {
		return life_;
	}

	void setDamage(int dmg) {
		life_ -= dmg;

		if (life_ < 0) life_ = 0;
	}

	bool isOnGround() {
		return onGround_;
	}

	void setOnGround(bool set) {
		onGround_ = set;
	}

	float getMaxHp() {
		return maxHp_;
	}

	void setDefeated(bool set) {
		defeated_ = set;
	};

	bool isDefeated() {
		return defeated_;
	}
};

