#include "FramedImage.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"

#include "Transform.h"


//includes para ajustar animaciones, quitarlos al cambiar a JSON
#include "player/PlayerCtrl.h"
#include "boss/frog_boss/FrogJump.h"
#include "boss/tree_boss/RootWave.h"
#include "boss/finalBoss/Punch.h"
#include "boss/finalBoss/FinalBossMovement.h"

FramedImage::FramedImage(Texture* tex, int row, int column,float time, int numframes_=0, std::string anim = 0) : totalAnimationTime_(time), 
tr_(), tex_(tex), row_(row), column_(column),flipX_(false),numframes(numframes_), currentAnim(anim),noRepeat_(false), completed_(false),
slowed_(false), slowFactor_(1), contFramesSlowed_(-1), timer_(), visible_(true), adjustToTransform_(false)
{
	m_clip.w = tex_->width() / column;
	m_clip.h = tex_->height() / row;

	iniTotalAnimTime_ = totalAnimationTime_;

	
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

	timer_ = mngr_->addTimer();

	colorTimer_ = mngr_->addTimer();
}

void FramedImage::render()
{
	if (visible_) {

		if (!completed_) {

			select_sprite(i, j);

			if (timer_->currTime() >= totalAnimationTime_ / numframes) {

				if (i < column_ - 1) {
					i++;
				}
				else {
					i = 0;
					j++;
				}

				checkAnimationFinished();

				timer_->reset();
				currentnumframes++;

				//disminuye el contador de frames ralentizados
				if (contFramesSlowed_ > 0) {

					contFramesSlowed_--;

					if (contFramesSlowed_ == 0) cancelSlow();
				}

				//eventos
				checkEvents();
			}
		}

		adjustAndRenderFrame();
	}
}

void FramedImage::adjustAndRenderFrame()
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	//flip the sprite
	if (flipX_)
		flip = SDL_FLIP_HORIZONTAL;

	// Aplicar propiedades

	SDL_Rect dest;

	//ESCALA
	if (!adjustToTransform_) {

		//float offset
		float xOffset = 0;
		float yOffset = 0;

		calculateOffset(xOffset, yOffset);

		float height = m_clip.h * tr_->getScale();
		float width = m_clip.w * tr_->getScale();

		auto posX = tr_->getPos().getX() + xOffset * width;

		auto yAdjustment = tr_->getHeight() - height;
		auto posY = tr_->getPos().getY() + yAdjustment + yOffset * height;

		Vector2D pos = new Vector2D(posX, posY);
		pos = pos - mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>()->getPos();

		dest = build_sdlrect(pos, width, height);
		dest.x += xOffset;
		dest.y += yOffset;
	}

	else {
	
		auto posX = tr_->getPos().getX();
		auto posY = tr_->getPos().getY();

		float height = tr_->getHeight();
		float width = tr_->getWidth();

		Vector2D pos = new Vector2D(posX, posY);
		pos = pos - mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>()->getPos();

		dest = build_sdlrect(pos, width, height);
	}

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	dest.x *= sW;
	dest.w *= sW;
	dest.y *= sH;
	dest.h *= sH;

	assert(tex_ != nullptr);
	tex_->render(m_clip, dest, tr_->getRot(), nullptr, flip);
}

//temporal: deberia ser sustituido por atributos de un JSON
void FramedImage::calculateOffset(float& xOffset, float& yOffset)
{
	if (ent_->getComponent<PlayerCtrl>() != nullptr) 
	{
		if (flipX_) {
			if (currentAnim == "Chica_Idle") {
				xOffset = -0.40;
				yOffset = 0.08;
			}
			else if (currentAnim == "Chica_Jump") {
				xOffset = -0.35;
				yOffset = 0.1;
			}
			else if (currentAnim == "Chica_Run") {
				xOffset = -0.35;
				yOffset = 0.1;
			}
			else if (currentAnim == "Chica_AtkFloor1" || currentAnim == "Chica_AtkFloor2" || currentAnim == "Chica_AtkFloor3" || currentAnim == "Chica_AtkFinished") {
				xOffset = -0.4;
				yOffset = 0.145;
			}
			else if (currentAnim == "Attack1_Recovery") {
				xOffset = -0.4;
				yOffset = 0.1;
			}
			else if (currentAnim == "Chica_AtkAir1" || currentAnim == "Chica_AtkAir2" || currentAnim == "Chica_AtkAir3") {
				xOffset = -0.4;
				yOffset = 0.145;
			}
			else if (currentAnim == "chicaroll") {
				xOffset = -0.45;
				yOffset = 0.25;
			}
		}
		else {
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
			else if (currentAnim == "Chica_AtkFloor1" || currentAnim == "Chica_AtkFloor2" || currentAnim == "Chica_AtkFloor3" || currentAnim == "Chica_AtkFinished") {
				xOffset = -0.4;
				yOffset = 0.145;
			}
			else if (currentAnim == "Attack1_Recovery") {
 				xOffset = -0.35;
				yOffset = 0.1;
			}
			else if (currentAnim == "Chica_AtkAir1" || currentAnim == "Chica_AtkAir2" || currentAnim == "Chica_AtkAir3") {
				xOffset = -0.4;
				yOffset = 0.145;
			}
			else if (currentAnim == "chicaroll") {
				xOffset = -0.25;
				yOffset = 0.25;
			}
		}	
	}
	
	else if (ent_->getComponent<FrogJump>() != nullptr)
	{
		if (currentAnim == "rana_idle" || currentAnim == "rana_enfadada_idle" || currentAnim == "rana_jump" || currentAnim == "rana_enfadada_jump" 
			|| currentAnim == "rana_lengua" || currentAnim == "rana_enfadada_lengua" || currentAnim == "rana_cambio_fase" 
			|| currentAnim == "rana_salto_a_vulnerable" || currentAnim == "rana_vulnerable" || currentAnim == "rana_vulnerable_a_idle" 
			|| currentAnim == "rana_enfadada_muerte") {
			xOffset = 0;
			yOffset = 0.25;
		}
	}

	else if (ent_->getComponent<FinalBossMovement>() != nullptr) {

		if (currentAnim == "FinalBoss_Fase1" ) {
			xOffset = -.17;
			yOffset = .25;
		}
	}

	if (currentAnim == "bolaFuego") {

	}
}

void FramedImage::checkAnimationFinished()
{
	if ((currentnumframes >= numframes - 1)) {

		j = 0; i = 0; currentnumframes = 0;
		if (noRepeat_) {
			completed_ = true;
		}

		//elimina los eventos registrados
		clearEvents();
	}
}

void FramedImage::checkEvents()
{
	for (auto i = 0u; i < eventsInfo_.size(); i++) {

		//si la animación del evento es la actual y ha llegado al numero de frames llama al callback
		if (currentAnim == eventsInfo_[i].second && currentnumframes == eventsInfo_[i].first)
			eventsCallbacks_[i]();
	}
}

void FramedImage::flipX(bool s)
{
	flipX_ = s;
}

bool FramedImage::isFlipped()
{
	return flipX_;
}

void FramedImage::repeat(bool h)
{
	noRepeat_ = !h;
	completed_ = false;
}

bool FramedImage::hasFinished()
{
	return completed_;
}

void FramedImage::slowAnimation(float factor, int nFrames)
{
	slowed_ = true;
	slowFactor_ = factor;
	contFramesSlowed_ = nFrames;

	totalAnimationTime_ = iniTotalAnimTime_ * slowFactor_;
}

void FramedImage::cancelSlow()
{
	slowed_ = false;

	totalAnimationTime_  = iniTotalAnimTime_;
}

void FramedImage::setVisible(bool set)
{
	visible_ = set;
}

bool FramedImage::isVisible()
{
	return visible_;
}

void FramedImage::setAlpha(int num)
{
	alpha_ = num;
	assert(alpha_ >= 0 && alpha_ <= 255);
	tex_->setAlpha(alpha_);
}

void FramedImage::setBlendMode(SDL_BlendMode blending)
{
	tex_->setBlendMode(blending);
}

void FramedImage::adjustToTransform(bool set)
{
	adjustToTransform_ = set;
}

void FramedImage::reset()
{
	j = 0;
	i = 0;
	currentnumframes = 0;
	completed_ = false;

	cancelSlow();
	clearEvents();
}

void FramedImage::changeanim(Texture* tex, int row, int column, float time, int numframes_, std::string newAnim)
{
	if (currentAnim == newAnim) return;

	totalAnimationTime_ = time;
	iniTotalAnimTime_ = totalAnimationTime_;
	tex_ = tex;
	row_ = row;
	column_ = column;
	currentnumframes = 0;
	numframes = numframes_;
	i = j = 0;
	m_clip.w = tex_->width() / column;
	m_clip.h = tex_->height() / row;

	currentAnim = newAnim;

	completed_ = false;

	timer_->reset();
}
void FramedImage::registerEvent(std::pair<int, std::string> eventInfo, std::function<void()> callback)
{
	eventsInfo_.push_back(eventInfo);
	eventsCallbacks_.push_back(callback);
}
int FramedImage::getFrameNum() 
{
	return currentnumframes;
}

void FramedImage::clearEvents()
{
	eventsInfo_.clear();
	eventsCallbacks_.clear();
}

void FramedImage::fadeIn()
{
	isFadingOut_ = false;
	isFadingIn_ = true;
}

void FramedImage::fadeOut()
{
	isFadingIn_ = false;
	isFadingOut_ = true;
}

void FramedImage::setColor(Uint8 r, Uint8 g, Uint8 b, int duration) {
	red_ = r;
	green_ = g;
	blue_ = b;
	colorDuration_ = duration;
	colorTimer_->reset();
}

void FramedImage::update() {

	if (colorDuration_ != -1)
	{
		if (colorTimer_->currTime() >= colorDuration_) {
			red_ = 255;
			green_ = 255;
			blue_ = 255;
		}
	}
	getTexture()->setColor(red_, green_, blue_);
	if (isFadingIn_) {
		if (alpha_ >= 255) {
			alpha_ = 255;
			isFadingIn_ = false;
		}
		else {
			alpha_++;
			setAlpha(alpha_);
		}
	}
	if (isFadingOut_) {
		if (alpha_ <= 0) {
			alpha_ = 0;
			isFadingOut_ = false;
		}
		else {
			alpha_--;
			setAlpha(alpha_);
		}
	}
}