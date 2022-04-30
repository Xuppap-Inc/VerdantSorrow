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
#include "ControlsScene.h"
#include "Game.h"
#include "../components/fondos/ParticleSystem.h"

MenuScene::MenuScene():BaseMenu(),mouseIndex_(-1),controllerIndex_(-1),delay_(250), lastUpdate_(0),
					   buttonsActive_(false), background_(), background1_(), background2_()
{

}

void MenuScene::init()
{
	buttonsActive_ = false;
	changeSc_ = false;
	Scene::init();
	isChangingScene(changeSc_);
	background();//Dibuja el fondo
	
}

void MenuScene::background()
{
	background_ = mngr_->addEntity();
	background_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	background_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondo"));
	background_->addToGroup(ecs::_BACKGROUND_1_GRP);

	background_ = mngr_->addEntity();
	background_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	background_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondoLogo"));
	background_->addToGroup(ecs::_BACKGROUND_2_GRP);

	background1_ = mngr_->addEntity();
	background1_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	background1Img_ = background1_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondo1"));
	background1Img_->setBlendMode(SDL_BLENDMODE_BLEND);
	background1Img_->setAlpha(255);
	background1_->addToGroup(ecs::_UI_GRP);

	background2_ = mngr_->addEntity();
	background2_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	background2Img_= background2_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondo2"));
	background2Img_->setBlendMode(SDL_BLENDMODE_BLEND);
	background2Img_->setAlpha(255);
	background2_->addToGroup(ecs::_UI_GRP);

	particlesys_ = new ParticleSystem(&sdlutils().imagesHub().at("particula_menu"), mngr_);
	particlesys_->createParticlesMenu(30);

	particlesys2_ = new ParticleSystem(&sdlutils().imagesHub().at("particula_menu2"), mngr_);
	particlesys2_->createParticlesMenu2(80);
}

void MenuScene::update()
{
	if (buttonsActive_) {
		handleInput(buttonPositions_,delay_,lastUpdate_,controllerIndex_,buttonNames_, buttonPoperties_); //Metodo para control de input 
	}
	else
	{
		auto& ihdlr = ih();
		if (ihdlr.isKeyDown(SDLK_SPACE)) {
			
			generateAllButtons();
			buttonsActive_ = true;

			background2Img_->setAlpha(0);
			background1Img_->fadeOut();
			particlesys_->disolveParticles();

			SoundEffect* s = &sdlutils().soundEffectsHub().at("sfx_iniciar_menu");
			s->play();
		}
	}
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
	isChangingScene(changeSc_);
	switch (index)
	{
	case 0: //Boton new game

		sC().changeScene(SceneManager::Hub_);
		
		break;
	case 1: //Boton continue


		break;
	case 2: //Boton controls
		sC().changeScene(SceneManager::Controls_);
		break;
	case 3: //Boton quit
		Game::instance()->setExit(true);
		break;
	default:
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
		createButton(sdlutils().width() / 2 - buttonW, sdlutils().height() / 2 - offsetY + (i * spacingY) + 100,
			buttonW, buttonH, buttonNames_[i], buttonPositions_,buttonPoperties_);
	}
	int j = 0; //Variable para separar los botones en su posicion Y

	//Bucle que dibuja la columna de la derecha
	for (int i = rows; i < buttonNames_.size(); ++i)
	{

		createButton(sdlutils().width() / 2 + spacingX, sdlutils().height() / 2 - offsetY + (j * spacingY) + 100,
			buttonW, buttonH, buttonNames_[i],buttonPositions_, buttonPoperties_);
		++j;
	}

}



