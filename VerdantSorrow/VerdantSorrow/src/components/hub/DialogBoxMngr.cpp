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


DialogBoxMngr::DialogBoxMngr(std::string font) :tr_(), font_(font), index(), letterWidth_(13), letterHeight_(28), lineOffsetY_(2), letterTimer_(50), lastChar_("")
{
}

void DialogBoxMngr::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	vt_ = new VirtualTimer();
}

void DialogBoxMngr::update()
{
	//auto& ihdlr = ih();

	//if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_E)) {
	//	next();
	//}

	if (vt_->currTime() > letterTimer_) {
		if (finished_) {

		}
		else
			addLetter();

		vt_->reset();
	}
}

void DialogBoxMngr::render()
{
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(build_sdlcolor(0xFFFFFFFF)));

	SDL_Rect rect = build_sdlrect(tr_->getPos().getX(), tr_->getPos().getY(), tr_->getWidth(), tr_->getHeight());

	SDL_RenderFillRect(sdlutils().renderer(), &rect);

	int i = 0;
	while (i < lines_.size()) {
		Texture text(sdlutils().renderer(), lines_[i], sdlutils().fonts().at(font_), build_sdlcolor(0x444444ff));
		SDL_Rect dest = build_sdlrect(tr_->getPos().getX(), tr_->getPos().getY() + ((letterHeight_ + lineOffsetY_) * i), text.width(), text.height());
		text.render(dest);
		i++;
	}
}

void DialogBoxMngr::activate(std::string dialog)
{
	if (!ent_->isActive()) {
		/*index = 0;
		divideText(dialog);*/
		dialog_ = dialog;
		ent_->setActive(true);
	}
}

void DialogBoxMngr::divideText(std::string dialog)
{
	//limpia dialogs_
	int n = dialogs_.size();
	for (int i = 0; i < n; i++)
		dialogs_.pop_back();

	int numberLines = (int)tr_->getHeight() / letterHeight_;
	int lettersPerLine = (int)tr_->getWidth() / letterWidth_;

	//divide el dialogo en palabras
	std::vector<std::string> dividedDialog;
	std::string word;
	std::stringstream X(dialog);

	while (std::getline(X, word, ' '))
		dividedDialog.push_back(word);

	//divide el dialogo (ya dividido en palabras) en lineas de tama�o <= maxLettersRect
	std::string line = "";
	for (auto d : dividedDialog) {
		if ((line.size() + d.size()) > lettersPerLine) {
			dialogs_.push_back({ sdlutils().renderer(), line, sdlutils().fonts().at(font_), build_sdlcolor(0x444444ff) });
			line = "";
		}

		line += " " + d;
	}
	dialogs_.push_back({ sdlutils().renderer(), line, sdlutils().fonts().at(font_), build_sdlcolor(0x444444ff) });
}


void DialogBoxMngr::next()
{
	index += (int)tr_->getHeight() / letterHeight_;

	if (index >= dialogs_.size()) {
		ent_->setActive(false);
	}
}

void DialogBoxMngr::addLetter()
{
	if (dialog_ == "") {
		finished_ = true;
		return;
	}

	if (lineNumber_ >= lines_.size())
		lines_.push_back(" ");

	//solo cabe una letra mas, hay más de una letra en el string
	if (lines_[lineNumber_].size() * letterWidth_ >= tr_->getWidth() - letterWidth_ && dialog_.size() > 1) {//line width

		if (dialog_[0] == ' ') {
			lines_[lineNumber_] += " ";
			dialog_.erase(0, 1);
		}
		else if(lastChar_ != " ")
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
