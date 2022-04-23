// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <cassert>

class Transform : public ecs::Component {
public:

	// This line expands to the following (see the defintion of
	// __CMPID_DECL__ en ecs.h):
	//
	//    constexpr static ecs::cmpId_type id = ecs::_TRANSFORM
	//
	__CMPID_DECL__(ecs::_TRANSFORM)
		/* g establece si hay gravedad en el jugador para las colisiones en la parte 
		baja de la pantalla en el hub*/
		Transform() :
		pos_(), vel_(), width_(), height_(), rot_(), gravity_() {
	}

	Transform(Vector2D pos, Vector2D vel, float w, float h, float r) :
		pos_(pos), vel_(vel), width_(w), height_(h), rot_(r), scale_(1) {
	}

	virtual ~Transform() {
	}

	void init(Vector2D pos, Vector2D vel, float w, float h, float r, float scale = 1, bool g = true) {
		pos_ = pos;
		vel_ = vel;
		width_ = w;
		height_ = h;
		rot_ = r;
		gravity_ = g;
		
		scale_ = scale;
	}

	Vector2D& getPos() {
		return pos_;
	}
	Vector2D& getVel() {
		return vel_;
	}

	float getWidth() {
		return width_;
	}

	void setWidth(float w) {
		width_ = w;
	}

	float getHeight() {
		return height_;
	}

	void setHeight(float h) {
		height_ = h;
	}

	float getRot() {
		return rot_;
	}

	void setRot(float r) {
		rot_ = r;
	}

	void update() override {
		pos_ = pos_ + vel_;
	}

	bool getGravity() {
		return gravity_;
	}

	void setScale(float scale) {
		scale_ = scale;
	}

	float getScale() {
		return scale_;
	}

protected:
	Vector2D pos_;
	Vector2D vel_;
	float width_;
	float height_;
	float rot_;
	bool gravity_;

	float scale_;
};

