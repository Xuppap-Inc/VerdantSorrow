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
	background();//Dibuja el fondo

	//Vector con los nombres de los botones
	std::vector<std::string> buttonNames = { "newGame", "continue", "settings", "controls","quit" };

	//Variables que definen caracteristicas de los botones
	int offsetY = 40; int spacingX = 20; int spacingY = 100;
	int buttonW = 200; int buttonH = 80;

	//Bucle que dibuja la primera columna (izq) de botones
	for (int i = 0; i < 2; ++i) 
	{
		createButton(sdlutils().width() / 2 - buttonW, sdlutils().height() / 2-offsetY+(i*spacingY), buttonW, buttonH, buttonNames[i]);
	}
	int j = 0; //Variable para separar los botones en su posicion Y

	//Bucle que dibuja la columna de la derecha
	for (int i = 2; i < buttonNames.size(); ++i)
	{
		
		createButton(sdlutils().width() / 2+spacingX, sdlutils().height() / 2-offsetY+(j*spacingY), buttonW, buttonH, buttonNames[i]);
		++j;
	}




}

void MenuScene::background()
{
	Scene::background("fondoMenu");
}

void MenuScene::createButton(float x, float y, float w, float h, std::string buttonImage)
{
	//Añade un boton como entidad, y le da una posicion y una imagen
	auto newButton = mngr_->addEntity();
	auto tr = newButton->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newButton->addComponent<Image>(&sdlutils().images().at(buttonImage));
	buttonPositions.push_back(tr);
	allButtons.push_back(newButton);
}

void MenuScene::update()
{
	auto& ihdlr = ih();
	
	if (ihdlr.mouseButtonEvent()) { //Booleano que comprueba eventos de ratón
	
		if (ihdlr.getMouseButtonState(ihdlr.LEFT)) //Comprueba si hace click izquierdo
		{
			auto ratonPos = ihdlr.getMousePos();
			for (auto buttonPos : buttonPositions) //Para todos los botones comprueba si el raton esta sobre ellos
			{
				auto pos = buttonPos->getPos();
				if (ratonPos.first <=  pos.getX()+ buttonPos->getWidth() 
					&& ratonPos.first >= pos.getX()&& ratonPos.second <= pos.getY() 
					+ buttonPos->getHeight() && ratonPos.second >= pos.getY()) {

					//onButtonClicked(pos);
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

void MenuScene::onButtonClicked(Vector2D buttonPos)
{
	/*std::string name;
	for (auto button : allButtons) 
	{
		if (button->getComponent<Transform>()->getPos() == buttonPos) 
		{
			
		}
	}*/
}
	

