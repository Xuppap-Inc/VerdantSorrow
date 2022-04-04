#pragma once

class Texture;
class Capas
{
public:
	Capas(Texture* t, float velocity);
	float getvel() {
		return velocity_;
	}
	Texture* gettex() {
		return tex_;
	}

private:
	float velocity_;
	Texture* tex_;
};

