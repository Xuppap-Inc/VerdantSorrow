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

	void slowAnimation(float factor, int nFrames = -1);
	void cancelSlow();

	void changeanim(Texture* tex, int row, int column, float time, int numframes_, std::string newAnim);

	int getFrameNum();
	std::string getCurrentAnimation() { return currentAnim; }

private:
	Transform* tr_;
	Texture* tex_;
	float totalAnimationTime;
	int row_;
	int column_;
	SDL_Rect m_clip;
	int i = 0;
	int j = 0;
	bool flipX_;
	int numframes;
	int currentnumframes = 0;
	bool noRepeat_;
	bool completed_;
	std::string currentAnim;

	VirtualTimer timer_;

	float slowFactor_;
	bool slowed_;
	int contFramesSlowed_;
};

