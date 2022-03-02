#pragma once
class BossAtributos :
	public ecs::Component
{
private:
	float life_;
	bool onGround_;
	bool onRightBorder_;
	bool onLeftBorder_;
public:
	__CMPID_DECL__(ecs::_BOSS_ATTRIBUTES)
	BossAtributos() : life_(), onGround_(false), onRightBorder_(false), onLeftBorder_(false) {};
	BossAtributos(float vida) : life_(vida), onGround_(false), onRightBorder_(false), onLeftBorder_(false)	{};

	float getLife() {
		return life_;
	}

	void setDamage(float l) {
		life_ -= l;
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
};

