#include "FramedImage.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"


#include "Transform.h"


FramedImage::FramedImage(Texture* tex, int row, int column,float time, int numframes_=0, std::string anim = 0) : frametime(time), tr_(), tex_(tex), row_(row), column_(column),flipX_(false),numframes(numframes_), currentAnim(anim),repeat_(false), repeating_(false)
{
	m_clip.w = tex_->width() / column;
	m_clip.h = tex_->height() / row;
	initime = sdlutils().currRealTime();
}

FramedImage::~FramedImage()
{
}

void FramedImage::select_sprite(int x, int y)
{
	m_clip.x = x * m_clip.w;
	m_clip.y = y * m_clip.h;
}

void FramedImage::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void FramedImage::render()
{
	if (!repeating_)
		select_sprite(i, j);

	if (!repeating_ && sdlutils().currRealTime() - initime >= frametime/numframes) {

		select_sprite(i, j);

		currentnumframes++;
		if (i < column_ - 1) { 
			i++; 	
		}
		else {
			i = 0;
			j++;
		}
		if ((currentnumframes >= numframes - 1)) { 
			j = 0; i = 0; currentnumframes = 0;
		    if (repeat_) {
			  repeating_ = true;
			}

			/*if (currentAnim == "Chica_AtkFloor")
				currentAnim = "Chica_AtkFinished";*/
			//repeating_ = false;
		}

		initime = sdlutils().currRealTime();
	}
	SDL_RendererFlip flip= SDL_FLIP_NONE;
	//flip the sprite
	if (flipX_)
		flip = SDL_FLIP_HORIZONTAL;


	//SDL_Rect dest;
	//SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getWidth(),
	//	tr_->getHeight());

	float multiplierX = 1;
	float multiplierY = 1;
	//float offset
	float xOffset = 0;
	float yOffset = 0;

	if (currentAnim == "Chica_Idle") {
		multiplierX = 1.4f;
		multiplierY = 1.3f;
		xOffset = -10;
		yOffset = -20;
		if (flipX_) { xOffset = -30; }
	}
	else if (currentAnim == "Chica_Jump") {
		multiplierX = 1.5f;
		multiplierY = 1.3f;
		xOffset = -30;
		yOffset = -20;
		if (flipX_) { xOffset = -15; }
	}
	else if (currentAnim == "Chica_Run") {
		multiplierX = 1.6f;
		multiplierY = 1.3f;
		xOffset = -40;
		yOffset = -20;
		if (flipX_) { xOffset = -20; }
	}
	else if (currentAnim == "Chica_AtkFloor" || currentAnim == "Chica_AtkFinished") {
		multiplierX = 1.7f;
		multiplierY = 1.2f;
		xOffset = -25;
		yOffset = -7;
		if (flipX_) { xOffset = -45; }
	}
	else if (currentAnim == "Chica_AtkAir") {
		multiplierX = 1.6f;
		multiplierY = 1.2f;
		xOffset = 0;
		yOffset = -7;
		if (flipX_) { xOffset = -60; }
	}

	// Aplicar propiedades

	auto posX = tr_->getPos().getX();

	auto yAdjustment = tr_->getHeight() - m_clip.h;
	auto posY = tr_->getPos().getY() + yAdjustment;

	Vector2D pos = new Vector2D(posX, posY);

	SDL_Rect dest = build_sdlrect(pos, m_clip.w,
		m_clip.h);
	dest.x += xOffset;
	dest.y += yOffset;

	assert(tex_ != nullptr);
	tex_->render(m_clip, dest, tr_->getRot(),nullptr,flip);
}

void FramedImage::flipX(bool s)
{
	flipX_ = s;
}

void FramedImage::repeat(bool h)
{
	repeat_ = h;
	repeating_ = false;
}

void FramedImage::changeanim(Texture* tex, int row, int column, float time, int numframes_, std::string newAnim)
{
	frametime = time;
	tex_ = tex;
	row_ = row;
	column_ = column;
	currentnumframes = 0;
	numframes = numframes_;
	i = j = 0;
	initime = sdlutils().currRealTime();
	m_clip.w = tex_->width() / column;
	m_clip.h = tex_->height() / row;

	currentAnim = newAnim;
}
int FramedImage::getFrameNum() 
{
	return currentnumframes;
}