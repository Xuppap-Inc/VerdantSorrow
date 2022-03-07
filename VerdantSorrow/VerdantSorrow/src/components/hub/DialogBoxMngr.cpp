#include "DialogBoxMngr.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include <sstream>
#include <string>
#include <iostream>


DialogBoxMngr::DialogBoxMngr(std::string font):tr_(),font_(font),index()
{
	Texture text(sdlutils().renderer(), "a",sdlutils().fonts().at(font), build_sdlcolor(0x00000000));//cada letra tiene un tamaño distinto, ojo
	letterWidth_ = text.width();
	letterHeight_ = text.height();
}

void DialogBoxMngr::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void DialogBoxMngr::update()
{
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_E)) {
		next();
	}
}

void DialogBoxMngr::render()
{
	int numberLines = (int)tr_->getHeight() / letterHeight_;
	int i = index;
	while (i < index + numberLines && i < dialogs_.size()) {
		SDL_Rect dest = build_sdlrect(tr_->getPos().getX(), tr_->getPos().getY() + (dialogs_[i].height() * (i-index)), dialogs_[i].width(), dialogs_[i].height());
		dialogs_[i].render(dest);
		i++;
	}
}

void DialogBoxMngr::activate(std::string dialog)
{
	if (!ent_->isActive()) {
		index = 0;
		divideText(dialog);
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

	while(std::getline(X,word,' '))
		dividedDialog.push_back(word);

	//divide el dialogo (ya dividido en palabras) en lineas de tamaño <= maxLettersRect
	std::string line = "";
	for (auto d : dividedDialog) {
		if ((line.size() + d.size()) > lettersPerLine) {
			dialogs_.push_back({ sdlutils().renderer(), line, sdlutils().fonts().at(font_), build_sdlcolor(0x444444ff) });
			line = "";
		}
		
		line += " " + d ;
	}
	dialogs_.push_back({ sdlutils().renderer(), line, sdlutils().fonts().at(font_), build_sdlcolor(0x444444ff) });
}

void DialogBoxMngr::next()
{
	index += (int)tr_->getHeight() / letterHeight_;
	
	if(index >= dialogs_.size())
		ent_->setActive(false);	
}
