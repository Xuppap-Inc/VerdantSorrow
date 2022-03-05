
#pragma once
#include "../ecs/Component.h"
class Transform;
class Texture;


#include "../sdlutils/SDLUtils.h"

class FramedImage : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_FRAMEDIMAGE)


		FramedImage(Texture* tex, int row, int column,float time,int numframes, std::string anim);
	virtual ~FramedImage();

	void setTexture(Texture* tex) {
		tex_ = tex;
	}
	void select_sprite(int x, int y);
	void initComponent() override;
	void render() override;
	void flipX(bool h);
	void repeat(bool h);

	void changeanim(Texture* tex, int row, int column, float time, int numframes_);

private:
	Transform* tr_;
	Texture* tex_;
	float frametime;
	int row_;
	int column_;
	SDL_Rect m_clip;
	int i = 0;
	int j = 0;
	bool flipX_;
	float initime = 0;
	int numframes;
	int currentnumframes = 0;
	bool repeat_;
	bool repeating_;
	std::string currentAnim;
};

