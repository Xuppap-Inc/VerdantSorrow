#pragma once
#include "../ecs/Component.h"
class Transform;
class Texture;


#include "../sdlutils/SDLUtils.h"
#include <vector>
#include <functional>

class FramedImage : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_FRAMEDIMAGE)


	FramedImage(Texture* tex, int row, int column,float time,int numframes, std::string anim);
	virtual ~FramedImage();

	void setTexture(Texture* tex) {
		tex_ = tex;
	}
	Texture* getTexture() {
		return tex_;
	}
	void select_sprite(int x, int y);
	void initComponent() override;
	void render() override;
	void flipX(bool h);
	void repeat(bool h);

	bool hasFinished();

	void slowAnimation(float factor, int nFrames = -1);
	void cancelSlow();

	void setVisible(bool set);
	bool isVisible();

	//Establece si la animación se ajusta al transform de la entidad
	void adjustToTransform(bool set);

	void reset();

	void changeanim(Texture* tex, int row, int column, float time, int numframes_, std::string newAnim);

	void registerEvent(std::pair<int, std::string> eventInfo, std::function<void()> callback);

	int getFrameNum();
	std::string getCurrentAnimation() { return currentAnim; }

	void setColor(Uint8 r, Uint8 g, Uint8 b, int duration = -1);

	void update() override;

private:
	Transform* tr_;
	Texture* tex_;
	float totalAnimationTime_;
	float iniTotalAnimTime_;
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

	bool visible_;
	bool adjustToTransform_;

	VirtualTimer timer_;

	float slowFactor_;
	bool slowed_;
	int contFramesSlowed_;

	std::vector<std::pair<int, std::string>> eventsInfo_;
	std::vector<std::function<void()>> eventsCallbacks_;

	void adjustAndRenderFrame();
	void calculateOffset(float& xOffset, float& yOffset);
	void checkAnimationFinished();
	void checkEvents();
	void clearEvents();

	VirtualTimer colorTimer_;
	int colorDuration_ = 0;
	int red_ = 255, green_ = 255, blue_ = 255;
};

