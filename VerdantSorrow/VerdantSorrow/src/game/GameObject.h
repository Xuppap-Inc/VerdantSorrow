#pragma once

#include <SDL.h>
class GameObject
{
public:
	GameObject() :
		width_(), height_(), rot_(){
	}

	virtual ~GameObject() {}

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	inline float getHeight() {
		return height_;
	}
	inline void setHeight(float height) {
		height_ = height;
	}

	inline float getWidth() {
		return width_;
	}
	inline void setWidth(float width) {
		width_ = width;
	}

	inline float getRotation() {
		return rot_;
	}
	inline void setHeight(float rot) {
		rot_ = rot;
	}
private:
	float width_;
	float height_;
	float rot_;
};

