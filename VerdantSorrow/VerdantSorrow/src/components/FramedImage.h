
#pragma once
#include "../ecs/Component.h"
class Transform;
class Texture;


#include "../sdlutils/SDLUtils.h"

class FramedImage : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_FRAMEDIMAGE)


		FramedImage(Texture* tex, int row, int column);
	virtual ~FramedImage();

	void setTexture(Texture* tex) {
		tex_ = tex;
	}
	void select_sprite(int x, int y);
	void initComponent() override;
	void render() override;

private:
	Transform* tr_;
	Texture* tex_;
	float frametime;
	int row_;
	int column_;
	SDL_Rect m_clip;
	int i = 0;
	int j = 0;
	float initime = 0;

};

