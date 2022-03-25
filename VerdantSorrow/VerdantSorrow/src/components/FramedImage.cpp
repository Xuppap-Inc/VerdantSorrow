#include "FramedImage.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"


#include "Transform.h"


FramedImage::FramedImage(Texture* tex, int row, int column,float time, int numframes_=0, std::string anim = 0) : frametime(time), tr_(), tex_(tex), row_(row), column_(column),flipX_(false),numframes(numframes_), currentAnim(anim),noRepeat_(false), completed_(false)
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
	if (!completed_) {

		select_sprite(i, j);

		if (sdlutils().currRealTime() - initime >= frametime / numframes) {
			
			if (i < column_ - 1) {
				i++;
			}
			else {
				i = 0;
				j++;
			}
			if ((currentnumframes >= numframes - 1)) {

				j = 0; i = 0; currentnumframes = 0;
				if (noRepeat_) {
					completed_ = true;
				}

				if (currentAnim == "Chica_AtkFloor")
					currentAnim = "Chica_AtkFinished";
			}

			initime = sdlutils().currRealTime();
			currentnumframes++;
		}
	}
	SDL_RendererFlip flip= SDL_FLIP_NONE;
	//flip the sprite
	if (flipX_)
		flip = SDL_FLIP_HORIZONTAL;

	//float offset
	float xOffset = 0;
	float yOffset = 0;

	if (currentAnim == "Chica_Idle") {
		xOffset = -0.25;
		yOffset = 0.08;
	}
	else if (currentAnim == "Chica_Jump") {
		xOffset = -0.25;
		yOffset = 0.1;
	}
	else if (currentAnim == "Chica_Run") {
		xOffset = -0.25;
		yOffset = 0.1;
	}
	else if (currentAnim == "Chica_AtkFloor" || currentAnim == "Chica_AtkFinished") {
		xOffset = -0.45;
		yOffset = 0.145;
	}
	else if (currentAnim == "Chica_AtkAir") {
		xOffset = -0.25;
		yOffset = 0;
	}
	else if (currentAnim == "chicaroll") {
		xOffset = -0.25;
		yOffset = 0.25;
	}

	// Aplicar propiedades

	//ESCALA
	float height = m_clip.h * tr_->getScale();
	float width = m_clip.w * tr_->getScale();


	auto posX = tr_->getPos().getX() + xOffset*width;

	auto yAdjustment = tr_->getHeight() - height;
	auto posY = tr_->getPos().getY() + yAdjustment + yOffset*height;

	Vector2D pos = new Vector2D(posX, posY);

	SDL_Rect dest = build_sdlrect(pos, width, height);
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
	noRepeat_ = h;
	completed_ = false;
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