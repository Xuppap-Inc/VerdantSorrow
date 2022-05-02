#include "DialogBoxMngr.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/VirtualTimer.h"
#include "../Transform.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/macros.h"
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "../player/PlayerHubControl.h"

using namespace std;


DialogBoxMngr::DialogBoxMngr(std::string font) :tr_(), font_(font), letterWidth_(14), letterHeight_(28),
												lineOffsetY_(2), letterTimer_(50), lastChar_(""), lastParagraph_(false), quickText_(false), finished_(false), lineNumber_(0)
												,skip_(false), cajaDialogo(&sdlutils().imagesHub().at("CajaDialogo"))
{
}

void DialogBoxMngr::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	vt_ = mngr_->addTimer();
	
}

void DialogBoxMngr::update()
{
	if (skip_) {
		while (!finished_)
			addLetter();
	}

	else if (!finished_ && (quickText_ || vt_->currTime() > letterTimer_)) {
		addLetter();
		vt_->reset();
	}
}

void DialogBoxMngr::render()
{
	//render sprite
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(build_sdlcolor(0xFFFFFFFF)));
	SDL_Rect rect = build_sdlrect(tr_->getPos().getX(), tr_->getPos().getY(), tr_->getWidth(), tr_->getHeight());

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	rect.x *= sW;
	rect.w *= sW;
	rect.y *= sH;
	rect.h *= sH;

	int w = 1000;
	int h = 300;
	SDL_Rect dialogoRect = build_sdlrect(tr_->getPos().getX() - w/5, tr_->getPos().getY() - h/2, w, h);

	cajaDialogo->render(dialogoRect); // 50 grosor

	//render text
	for (int i = 0; i < lines_.size(); i++) {
		Texture text(sdlutils().renderer(), lines_[i], sdlutils().fontsHub().at(font_), build_sdlcolor(0xffffffff));
		SDL_Rect dest = build_sdlrect(tr_->getPos().getX(), tr_->getPos().getY() + ((letterHeight_ + lineOffsetY_) * i) - 70, text.width(), text.height());

		//escalado pantalla
		dest.x *= sW;
		dest.w *= sW;
		dest.y *= sH;
		dest.h *= sH;

		text.render(dest);
	}
}

void DialogBoxMngr::activate(std::string dialog)
{
	if (!ent_->isActive()) {
		dialog_ = dialog;
		ent_->setActive(true);
	}
}

void DialogBoxMngr::desactivate()
{
	if (ent_->isActive()) {
		changeFinishedState();
		ent_->setActive(false);
		lastParagraph_ = false;
	}
}

void DialogBoxMngr::changeFinishedState()
{
	finished_ = false; lines_.clear(); lineNumber_ = 0; quickText_ = false; skip_ = false;
}

void DialogBoxMngr::addLetter()
{
	//dialogo ha terminado
	if (dialog_ == "") {
		lastParagraph_ = true;
		finished_ = true;
		return;
	}

	if (lineNumber_ >= lines_.size())
		lines_.push_back(" ");

	//solo cabe una letra mas, hay más de una letra en el dialog
	if (lines_[lineNumber_].size() * letterWidth_ >= tr_->getWidth() - letterWidth_ && dialog_.size() > 1) {//line width

		if (dialog_[0] == ' ') {
			lines_[lineNumber_] += dialog_[0];
			dialog_.erase(0, 1);
		}
		else if (lastChar_ != " ")
			lines_[lineNumber_] += "-";


		lineNumber_++;

		if (lineNumber_ * (letterHeight_ + lineOffsetY_) >= tr_->getHeight())
			finished_ = true;

		lastChar_ = "";
	}
	else {
		lines_[lineNumber_] += dialog_[0];
		dialog_.erase(0, 1);
		lastChar_ = lines_[lineNumber_].back();
	}
}