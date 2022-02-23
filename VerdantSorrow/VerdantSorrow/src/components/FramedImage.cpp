#include "FramedImage.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"


#include "Transform.h"


FramedImage::FramedImage(Texture* tex, int row, int column) : frametime(50), tr_(), tex_(tex), row_(row), column_(column)
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
	if (sdlutils().currRealTime() - initime >= 50) {
		if (i < column_ - 1)i++;
		else {
			i = 0;
			j++;
		}
		if (j >= row_)j = 0;
		initime = sdlutils().currRealTime();
	}

	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getWidth(),
		tr_->getHeight());

	assert(tex_ != nullptr);
	tex_->render(m_clip, dest, tr_->getRot());
}
