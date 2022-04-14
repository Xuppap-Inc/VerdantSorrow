#include "MenuScene.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "../utils/Vector2D.h"

MenuScene::MenuScene():BaseMenu(),mouseIndex_(-1),controllerIndex_(-1),delay_(250), lastUpdate_(0),changeSc_(false)
{

}

void MenuScene::init()
{
	Scene::init();
	changeSc_ = false;
	//background();//Dibuja el fondo

	generateAllButtons(); //Genera todos los botones del menu (para ordenar mejor el codigo)

}

//void MenuScene::background()
//{
//	Scene::background("fondoMenu");
//}

void MenuScene::update()
{
	handleInput(buttonPositions_,delay_,lastUpdate_,controllerIndex_,buttonNames_); //Metodo para control de input 
	if (!changeSc_) {
			mngr_->update();
			mngr_->refresh();
			sdlutils().clearRenderer();
			mngr_->render();
			mngr_->debug();
			sdlutils().presentRenderer();	
	}
}

void MenuScene::onButtonClicked(int index)
{
	/*Index indica la posicion en la que se encuentra el boton en el vector de transforms y de nombres
	/Los botones se añaden en el orden indicado en el vector de nombres
	/Cambiando el orden de los botones, cambiaria lo que hay que hacer en cada caso
	/De la misma manera si se añade un nuevo boton habría que añadir el caso correspondiente*/
	changeSc_ = true;
	switch (index)
	{
	case 0: //Boton new game
		std::cout << "new game";
		sC().changeScene(SceneManager::Hub_);
		
		break;
	case 1: //Boton continue


		break;
	case 2: //Boton controls
		std::cout << "controles";

		sC().changeScene(SceneManager::Controls_);
		break;
	case 3: //Boton quit
		SDL_Quit();
		break;
	default:
		std::cout << "nada" << std::endl;
		break;

	}

}

void MenuScene::generateAllButtons()
{
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int offsetY = 40, spacingX = 20, spacingY = 100, rows = 2;
	int buttonW = 200, buttonH = 80; // Width y Height de los botones

	//Bucle que dibuja la primera columna (izq) de botones
	for (int i = 0; i < rows; ++i)
	{
		createButton(sdlutils().width() / 2 - buttonW, sdlutils().height() / 2 - offsetY + (i * spacingY),
			buttonW, buttonH, buttonNames_[i], buttonPositions_);
	}
	int j = 0; //Variable para separar los botones en su posicion Y

	//Bucle que dibuja la columna de la derecha
	for (int i = rows; i < buttonNames_.size(); ++i)
	{

		createButton(sdlutils().width() / 2 + spacingX, sdlutils().height() / 2 - offsetY + (j * spacingY),
			buttonW, buttonH, buttonNames_[i],buttonPositions_);
		++j;
	}

}

//void MenuScene::selectButton(int index) //Metodo que cambia aspecto del boton cuando el cursor o mando estan sobre éste
//{
//	//Mouse index guarda el indice del boton sobre el que se encuentra el raton
//	mouseIndex_ = index;
//	auto image = buttonPoperties_[index]->getComponent<Image>();
//	image->setAlpha(127); //Baja la opacidad del boton
//
//}


//void MenuScene::deselectButton(int index) //Devuelve el aspecto original al boton cuando el cursor o mando dejan de estar sobre éste
//{
//	auto image = buttonPoperties_[index]->getComponent<Image>();
//	image->setAlpha(255);
//}


