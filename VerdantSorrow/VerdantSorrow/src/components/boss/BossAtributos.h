#pragma once
class BossAtributos :
	public ecs::Component
{
private:
	int life_;
	int maxHp_;
	bool onGround_;
	bool onRightBorder_;
	bool onLeftBorder_;
public:
	__CMPID_DECL__(ecs::_BOSS_ATTRIBUTES)
	BossAtributos() : life_(), maxHp_(), onGround_(false), onRightBorder_(false), onLeftBorder_(false) {};
	BossAtributos(int vida) : life_(vida), maxHp_(vida), onGround_(false), onRightBorder_(false), onLeftBorder_(false)	{};

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

	bool isOnRightBorder() {
		return onRightBorder_;
	}

	void setOnRightBorder(bool set){
		onRightBorder_ = set;
	}

	bool isOnLeftBorder() {
		return onLeftBorder_;
	}

	void setOnLeftBorder(bool set) {
		onLeftBorder_ = set;
	}

	float getMaxHp() {
		return maxHp_;
	}
};

