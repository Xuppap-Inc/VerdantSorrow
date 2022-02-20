#pragma once
#include "../Transform.h"
class BossAtributos : public Transform
{
private:
	float life_;
public:

	BossAtributos() :life_(), Transform() {};
	BossAtributos(Vector2D pos, Vector2D vel, float w, float h, float r, float vida) : life_(vida),
		Transform(pos, vel, w, h, r) {};

	float getLife() {
		return life_;
	}

	void setLife(float l) {
		life_ = l;
	}

	void init (Vector2D pos, Vector2D vel, float w, float h, float r, float l) {
		pos_ = pos;
		vel_ = vel;
		width_ = w;
		height_ = h;
		rot_ = r;
		life_ = l;
	}
};

