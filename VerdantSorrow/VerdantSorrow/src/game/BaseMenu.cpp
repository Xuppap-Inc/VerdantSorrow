#include "BaseMenu.h"
#include <string>
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../sdlutils/Texture.h"
#include "SceneManager.h"
#include "Scene.h"
#include "../utils/Vector2D.h"

BaseMenu::BaseMenu() :Scene(), changeSc_(false)
{

}

void BaseMenu::background(std::string backgroundName)
{
	Scene::background(backgroundName);
}

void BaseMenu::createButton(float x, float y, float w, float h, std::string buttImg, std::vector<Transform *>&buttPos, std::vector<Entity*>&buttProps)
{
	auto newButton = mngr_->addEntity();
	auto tr = newButton->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newButton->addComponent<Image>(&sdlutils().imagesHub().at(buttImg));
	buttPos.push_back(tr);
	buttProps.push_back(newButton);
}

void BaseMenu::handleInput(std::vector<Transform*>buttPos, float delay, float &lastUpdate, int &controllerIndex, std::vector<std::string>buttNames, std::vector<Entity*>buttProps)
{
	handleMouseInput(buttPos,buttProps);
	handleControllerInput(delay, lastUpdate, controllerIndex,buttNames,buttProps);
}

void BaseMenu::handleMouseInput(std::vector<Transform*>buttPos, std::vector<Entity*>buttProps)
{
	auto& ihdlr = ih();
	auto ratonPos = ihdlr.getMousePos();
	int i = 0;
	while (i < buttPos.size())
	{
		//Para todos los botones comprueba si el raton esta sobre ellos
		auto pos = buttPos[i]->getPos();
		auto x = pos.getX() * mngr_->getWindowScaleWidth();
		auto y = pos.getY() * mngr_->getWindowScaleHeight();

		pos = Vector2D(x, y);

		auto width = buttPos[i]->getWidth() * mngr_->getWindowScaleWidth();
		auto height = buttPos[i]->getHeight() * mngr_->getWindowScaleHeight();

		if (ratonPos.first <= pos.getX() + width
			&& ratonPos.first >= pos.getX()
			&& ratonPos.second <= pos.getY() + height
			&& ratonPos.second >= pos.getY()) {

			selectButton(buttProps[i]);

			if (ihdlr.mouseButtonEvent()) { //Booleano que comprueba eventos de ratón

				if (ihdlr.getMouseButtonState(ihdlr.LEFT)) //Comprueba si hace click izquierdo
				{
			
					onButtonClicked(i);
				
				}
			}
		}
		else  deselectButton(buttProps[i]);
		i++;
	}
}

void BaseMenu::handleControllerInput(float delay, float lastUpdate,int &controllerIndex, std::vector<std::string>buttNames, std::vector<Entity*>buttProps)
{
	auto& ihdlr = ih();
	if (ihdlr.controllerConnected()) //Input con el mando
	{
		if (ihdlr.isAxisMotionEvent())
		{
			if (delay + lastUpdate < sdlutils().currRealTime())
			{
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 0.9) //Movimiento hacia abajo
				{
					changeButton(controllerIndex, buttNames,lastUpdate,1); //Suma una posicion
				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 0.9) //Movimieto hacia la derecha
				{
					changeButton(controllerIndex, buttNames, lastUpdate, 2); //Suma tres posiciones (el menu es simetrico y hay tres botones por columna)

				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -0.9) //Movimiento hacia arriba
				{
					changeButton(controllerIndex, buttNames, lastUpdate, -1);
				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -0.9) //Movimiento hacia la izquierda
				{
					changeButton(controllerIndex, buttNames, lastUpdate, -2);
				}

			}
		}

		if (controllerIndex != -1 && controllerIndex < buttNames.size())
		{

			if (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A)) 
			{
		
				onButtonClicked(controllerIndex);
			
			}

			selectButton(buttProps[controllerIndex]);
		}
	}
}

void BaseMenu::selectButton(Entity*buttProps)
{
	if(!changeSc_)
	{
		auto img =buttProps->getComponent<Image>();
		img->setAlpha(127);
	}
	
	
}

void BaseMenu::changeButton(int &controllerIndex, std::vector<std::string>buttNames, float &lastUpdate,int numMoves)//Controla la lógica entre el cambio de botones seleccionados con el mando
{

	//Controller index guarda el indice del boton sobre el que se encuentra el raton
	
	if (controllerIndex< buttNames.size() || controllerIndex == -1)
	{
		controllerIndex += numMoves; //Suma un determinado numero de "movimientos" necesarios para llegar al boton deseado
	}
	else controllerIndex = -1;
	
	lastUpdate= sdlutils().currRealTime(); //Para controlar el delay entre cambio de botones con mando

}

void BaseMenu::deselectButton( Entity*buttProps)
{
	if(!changeSc_)
	{
		auto img = buttProps->getComponent<Image>();
		img->setAlpha(255);
	}

}
