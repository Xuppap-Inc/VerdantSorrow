#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../../sdlutils/Font.h"
#include "../../sdlutils/Texture.h"
class Transform;

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

	//muestra la parte del dialogo siguiente
	void next();
protected:

	Transform* tr_;
	std::string font_;

	int letterWidth_, letterHeight_;

	//dialogo dividido para su render
	std::vector<Texture> dialogs_;
	int index;
};

