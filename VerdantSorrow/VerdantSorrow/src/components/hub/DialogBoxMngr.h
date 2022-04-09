#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../../sdlutils/Font.h"
#include "../../sdlutils/VirtualTimer.h"
#include "../../sdlutils/Texture.h"

class Transform;
class VirtualTimer;

class DialogBoxMngr : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_DIALOG_BOX_MNGR)
		DialogBoxMngr(std::string font);
	~DialogBoxMngr() {};

	void initComponent() override;
	void update() override;
	void render() override;

	//activa el cuadro de dialogo
	void activate(std::string dialog);
	void desactivate();

	void changeFinishedState();

	bool isFinished() { return finished_; }

	bool isLastParagraph() { return lastParagraph_; }

	bool isQuick() { return quickText_; };

	void addLetter();

	void changeTextSpeed(bool set) { quickText_ = set; };
	void changeSkip(bool set) { skip_ = true; }

protected:

	//posicion del dialog box
	Transform* tr_;

	//fuente
	std::string font_;

	//tamaño de letra
	int letterWidth_, letterHeight_;

	//dialogo dividido en lineas
	std::vector<std::string> lines_;
	int lineNumber_;

	//flags para controlar la escritura del dialogo
	bool finished_, lastParagraph_;

	int lineOffsetY_;

	//timer de aparicion de letra
	int letterTimer_;
	VirtualTimer* vt_;

	//dialogo
	std::string dialog_;

	//ultimo caracter escrito
	std::string lastChar_;

	//texto rapido (0 cooldown)
	bool quickText_, skip_;
};