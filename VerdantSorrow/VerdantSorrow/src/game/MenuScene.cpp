#include "MenuScene.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"


MenuScene::MenuScene():Scene()
{

}

void MenuScene::init()
{
	Scene::init();
	background();
	createButton(sdlutils().width() / 2 - 100, sdlutils().height() / 2, 200, 100,"newGame");
}

void MenuScene::background()
{
	Scene::background("fondoMenu");
}

void MenuScene::createButton(float x, float y, float w, float h, std::string buttonImage)
{
	auto newGame = mngr_->addEntity();
	auto tr = newGame->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newGame->addComponent<Image>(&sdlutils().images().at(buttonImage));

	buttonPositions.push_back(tr);
	
}

void MenuScene::update()
{
	auto& ihdlr = ih();
	
	if (ihdlr.mouseButtonEvent()) {
	
		if (ihdlr.getMouseButtonState(ihdlr.LEFT))
		{
			auto ratonPos = ihdlr.getMousePos();
			for (auto buttonPos : buttonPositions) 
			{
				auto pos = buttonPos->getPos();
				if (ratonPos.first <=  pos.getX()+ buttonPos->getWidth() 
					&& ratonPos.first >= pos.getX()&& ratonPos.second <= pos.getY() 
					+ buttonPos->getHeight() && ratonPos.second >= pos.getY()) {
					std::cout << "BOTON PULSADO"<<std::endl;

				}
			}
			
		}
	}

	mngr_->update();
	mngr_->refresh();
	sdlutils().clearRenderer();
	mngr_->render();
	mngr_->debug();
	sdlutils().presentRenderer();
}
	

