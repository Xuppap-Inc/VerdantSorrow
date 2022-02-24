#include "FramedImage.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"


#include "Transform.h"


FramedImage::FramedImage():flipX_(false), tr_()
{
}

FramedImage::FramedImage(Texture* tex, int row, int column,float fametime_) : frametime(fametime_), tr_(), tex_(tex), row_(row), column_(column),flipX_(false)
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
	select_sprite(i, j);
	if (sdlutils().currRealTime() - initime >= frametime) {
		if (i < column_ - 1)i++;
		else {
			i = 0;
			j++;
		}
		if (j >= row_)j = 0;
		initime = sdlutils().currRealTime();
	}
	SDL_RendererFlip flip= SDL_FLIP_NONE;
	//flip the sprite
	if (flipX_) {
		flip = SDL_FLIP_HORIZONTAL;		
		
	}
	
	

	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getWidth(),
		tr_->getHeight());

	assert(tex_ != nullptr);
	tex_->render(m_clip, dest, tr_->getRot(),nullptr,flip);

}

void FramedImage::flipX(bool s)
{
	flipX_ = s;
}
