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


	//divide el dialogo para su render
	void divideText(std::string dialog);

	//muestra la parte del dialogo siguiente
	void next();

	void changeFinishedState(bool state);

	void changeLastParagraphState(bool state);

	bool isFinished() { return finished_; }

	bool isLastParagraph() { return lastParagraph_; }

	void addLetter();
protected:

	//posicion del dialog box
	Transform* tr_;

	//fuente
	std::string font_;

	int letterWidth_, letterHeight_, lastConversation_, conversationCooldown_;

	//dialogo dividido para su render
	std::vector<Texture> dialogs_;
	int index;

	std::vector<std::string> lines_;
	int lineNumber_;
	bool finished_, lastParagraph_;

	int lineOffsetY_;
	int letterTimer_;

	VirtualTimer* vt_;

	std::string dialog_;

	std::string lastChar_;
};

