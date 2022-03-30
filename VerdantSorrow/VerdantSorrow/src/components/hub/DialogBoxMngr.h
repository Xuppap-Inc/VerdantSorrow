#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../../sdlutils/Font.h"
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

	//divide el dialogo para su render
	void divideText(std::string dialog);

	bool canTalk();

	//muestra la parte del dialogo siguiente
	void next();



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
	bool finished_;

	int lineOffsetY_;
	int letterTimer_;

	VirtualTimer* vt_;

	std::string dialog_;
};

