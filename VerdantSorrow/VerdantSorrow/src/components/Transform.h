// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
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
	pos_(), vel_(), width_(), height_(), rot_(), gravity_(), scale_(1) {

		windowScaleHeight_ = sdlutils().height() / 1080.0f;
		windowScaleWidth_ = sdlutils().width() / 1920.0f;
	}

	Transform(Vector2D pos, Vector2D vel, float w, float h, float r) :
		pos_(pos), vel_(vel), width_(w), height_(h), rot_(r), scale_(1), gravity_() {

		windowScaleHeight_ = sdlutils().height() / 1080.0f;
		windowScaleWidth_ = sdlutils().width() / 1920.0f;
	}

	virtual ~Transform() {
	}

	void init(Vector2D pos, Vector2D vel, float w, float h, float r, float scale = 1, bool g = true) {
		
		auto x = pos.getX() * windowScaleWidth_;
		auto y = pos.getY() * windowScaleHeight_;
		pos_ = Vector2D(x, y);

		auto velX = vel.getX() * windowScaleWidth_;
		auto velY = vel.getY();
		vel_ = Vector2D(velX, velY);

		width_ = w * windowScaleWidth_;
		height_ = h * windowScaleHeight_;

		rot_ = r;
		gravity_ = g;
		
		scale_ = scale * windowScaleWidth_;
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
		width_ = w * windowScaleWidth_;
	}

	float getHeight() {
		return height_;
	}

	void setHeight(float h) {
		height_ = h * windowScaleHeight_;
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
		scale_ = scale * windowScaleWidth_;
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

	float windowScaleHeight_;
	float windowScaleWidth_;
};

