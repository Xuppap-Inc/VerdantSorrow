#include "MenuScene.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"


MenuScene::MenuScene() :Scene()
{

}

void MenuScene::init()
{
	Scene::init();

	//background();//Dibuja el fondo

	generateAllButtons(); //Genera todos los botones del menu (para ordenar mejor el codigo)

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
	buttonPositions_.push_back(tr);
	buttonPoperties_.push_back(newButton);
}

void MenuScene::update()
{

	handleInput(); //Metodo para control de input

	mngr_->update();
	mngr_->refresh();
	sdlutils().clearRenderer();
	mngr_->render();
	mngr_->debug();
	sdlutils().presentRenderer();
}

void MenuScene::onButtonClicked(int index)
{
	/*Index indica la posicion en la que se encuentra el boton en el vector de transforms y de nombres
	/Los botones se añaden en el orden indicado en el vector de nombres
	/Cambiando el orden de los botones, cambiaria lo que hay que hacer en cada caso
	/De la misma manera si se añade un nuevo boton habría que añadir el caso correspondiente*/

	switch (index)
	{
	case 0: //Boton new game
		std::cout << "Has pulsado el boton de nuevo juego" << std::endl;
		break;
	case 1: //Boton continue
		std::cout << "Has pulsado el boton de continuar" << std::endl;

		break;
	case 2: //Boton load boss
		std::cout << "Has pulsado el boton de load" << std::endl;

		break;
	case 3://Boton settings
		std::cout << "Has pulsado el boton de ajustes" << std::endl;

		break;
	case 4: //Boton controls
		std::cout << "Has pulsado el boton de controles" << std::endl;

		break;
	case 5: //Boton quit
		SDL_Quit();
		break;

	}

}

void MenuScene::generateAllButtons()
{
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int offsetY = 40, spacingX = 20, spacingY = 100, rows = 3;

	//Bucle que dibuja la primera columna (izq) de botones
	for (int i = 0; i < rows; ++i)
	{
		createButton(sdlutils().width() / 2 - buttonW_, sdlutils().height() / 2 - offsetY + (i * spacingY),
			buttonW_, buttonH_, buttonNames[i]);
	}
	int j = 0; //Variable para separar los botones en su posicion Y

	//Bucle que dibuja la columna de la derecha
	for (int i = rows; i < buttonNames.size(); ++i)
	{

		createButton(sdlutils().width() / 2 + spacingX, sdlutils().height() / 2 - offsetY + (j * spacingY),
			buttonW_, buttonH_, buttonNames[i]);
		++j;
	}

}

void MenuScene::handleInput()
{
	auto& ihdlr = ih();
	auto ratonPos = ihdlr.getMousePos();
	for (int i = 0; i < buttonPositions_.size(); ++i)
	{
		//Para todos los botones comprueba si el raton esta sobre ellos
		auto pos = buttonPositions_[i]->getPos();
		if (ratonPos.first <= pos.getX() + buttonPositions_[i]->getWidth()
			&& ratonPos.first >= pos.getX() && ratonPos.second <= pos.getY()
			+ buttonPositions_[i]->getHeight() && ratonPos.second >= pos.getY()) {

			selectButton(i); 

			if (ihdlr.mouseButtonEvent()) { //Booleano que comprueba eventos de ratón

				if (ihdlr.getMouseButtonState(ihdlr.LEFT)) //Comprueba si hace click izquierdo
				{
					onButtonClicked(i);
				}
			}
		}
		else  deselectButton(i);
	}

	if (ihdlr.controllerConnected()) //Input con el mando
	{
		if (ihdlr.isAxisMotionEvent())
		{
			if (delay_ + lastUpdate_ < sdlutils().currRealTime())
			{
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY) > 0.9) //Movimiento hacia abajo
				{
					changeButton(1); //Suma una posicion
				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX) > 0.9) //Movimieto hacia la derecha
				{
					changeButton(3); //Suma tres posiciones (el menu es simetrico y hay tres botones por columna)

				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY) < -0.9) //Movimiento hacia arriba
				{
					changeButton(-1);
				}
				if (ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX) < -0.9) //Movimiento hacia la izquierda
				{
					changeButton(-3);
				}
				

			}
		}
		if (controllerIndex_ != -1 && controllerIndex_ < buttonPositions_.size())
		{
			if(ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A)) onButtonClicked(controllerIndex_);

			selectButton(controllerIndex_);
		}
	}
}

void MenuScene::selectButton(int index) //Metodo que cambia aspecto del boton cuando el cursor o mando estan sobre éste
{
	//Mouse index guarda el indice del boton sobre el que se encuentra el raton
	mouseIndex_ = index;
	auto image=buttonPoperties_[index]->getComponent<Image>();
	image->setAlpha(127); //Baja la opacidad del boton

}


void MenuScene::changeButton(int moves) //Controla la lógica entre el cambio de botones seleccionados con el mando
{
	//Controller index guarda el indice del boton sobre el que se encuentra el raton

	if (controllerIndex_ < buttonNames.size() || controllerIndex_ == -1)
	{
		controllerIndex_ += moves; //Suma un determinado numero de "movimientos" necesarios para llegar al boton deseado
	}
	else controllerIndex_ = -1;

	lastUpdate_ = sdlutils().currRealTime(); //Para controlar el delay entre cambio de botones con mando
}

void MenuScene::deselectButton(int index) //Devuelve el aspecto original al boton cuando el cursor o mando dejan de estar sobre éste
{
	auto image = buttonPoperties_[index]->getComponent<Image>();
	image->setAlpha(255);
}


