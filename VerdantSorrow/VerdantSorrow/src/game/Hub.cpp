#include "Hub.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../components/RectangleRenderer.h"
#include "../components/RectangleCollider.h"
#include "../components/Transform.h"
#include "../components/FramedImage.h"
#include "../components/Image.h"
#include "../components/player/PlayerComponents.h"
#include "../components/player/PlayerHubControl.h"
#include "../components/hub/NpcCtrl.h"
#include "../components/hub/DialogBoxMngr.h"
#include "../components/Camera.h"

#include "CollisionManager.h"
#include "../game/SceneManager.h"
#include "../components/fondos/ParticleSystem.h"



Hub::Hub() :Scene()
{
	colManager = nullptr;
}

Hub::~Hub()
{

}

void Hub::init()
{
	Scene::init();

	//Para gestionar las colisiones
	colManager = new CollisionManager();

	changeSc = false;
	backgroundHub();
	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGenerator(colManager, player);
	auto camera = mngr_->addEntity();
	auto cameraTr = camera->addComponent<Transform>();
	cameraTr->init(Vector2D(0, 0), Vector2D(0, 0), 0, 0, 0);
	auto cameraC = camera->addComponent<Camera>();
	mngr_->setHandler(ecs::_hdlr_CAMERA, camera);
	//Genera las entradas a los bosses
		//entrada a la rana
	EntryGenerator(colManager, 0, 250);
	//Entrada al ojo
	EntryGenerator(colManager, sdlutils().width() - 100, 100);
	//Entrada al arbol
	EntryGenerator(colManager, sdlutils().width()-100, sdlutils().height()-100);
	auto dialogBox = mngr_->addEntity();
	dialogBoxGenerator(dialogBox);
	NPCGenerator(colManager, dialogBox);

	musica_ = &sdlutils().musics().at("musica_hub");
	musica_->play();
	musica_->setMusicVolume(60);
}

void Hub::dialogBoxGenerator(Entity* dialogBox)
{
	dialogBox->setActive(false);
	auto tr = dialogBox->addComponent<Transform>();
	tr->init(Vector2D((sdlutils().width() - 600) / 2, (sdlutils().height() - 200)), Vector2D(), 600, 150, 0.0f, false);
	dialogBox->addComponent<DialogBoxMngr>("PTMONO24");
}

bool Hub::getAble()
{
	return isAble;
}

void Hub::setAble(bool a)
{
	isAble = a;
}

void Hub::changeScene_(bool ch)
{
	changeSc = ch;
}

void Hub::checkCollissions()
{
	auto playerCol_ = player->getComponent<RectangleCollider>();
	if (colManager->hasCollisions(playerCol_)) {
		std::vector<RectangleCollider*> colliders = colManager->getCollisions(playerCol_);

		bool changeScene = false;
		int i = 0;
		while (!changeScene && i < colliders.size()) {
			changeScene = colliders[i]->isActive() && colliders[i]->isTrigger() && colliders[i]->getEntity()->getComponent<NpcCtrl>() == nullptr;
			i++;
		}
		if (changeScene) changeScene_(true);
	}
}

void Hub::backgroundHub()
{
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().images().at("fondoHub"));
}


void Hub::update()
{
	if (!changeSc) {
		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();

		checkCollissions();
	}
	else {
		sC().decideScene();
	}
}

void Hub::playerGenerator(CollisionManager* colManager, Entity* player_) {
	//Se le a�aden los atributos del player, no los del transform
	player_->addComponent<PlayerAttributes>();
	//Se le a�ade el transform
	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	//Se le dan las posiciones iniciales, vecocidad, ancho y alto al player
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 200, 200, 0.0f, false);

	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	//Se a�ade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	player_->addComponent<CollideWithBorders>();
	colManager->addCollider(playerCollider);
	//Componente que permite controlar al jugador
	player_->addComponent<PlayerHubControl>(3, colManager);

	//No poner estas f�sicas detr�s del playerctrl, se hunde y no funciona el salto
	//player_->addComponent<SimplePhysicsPlayer>(colManager);
	player_->addComponent<Image>(&sdlutils().images().at("chica"));


	//Componente ui jugador
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player);
}

void Hub::EntryGenerator(CollisionManager* colManager, float posX, float posY)
{
	auto frogEntry = mngr_->addEntity();

	auto frogEntryTr = frogEntry->addComponent<Transform>();
	auto frogEntryX = 0;
	auto frogEntryY = sdlutils().height() / 4 * 3;
	frogEntryTr->init(Vector2D(posX, posY), Vector2D(), 100, 100, 0.0f);

	frogEntry->addComponent<RectangleRenderer>();

	auto frogEntryCollider = frogEntry->addComponent<RectangleCollider>(frogEntryTr->getWidth(), frogEntryTr->getHeight());
	colManager->addCollider(frogEntryCollider);
	frogEntryCollider->setIsTrigger(true);
}

void Hub::NPCGenerator(CollisionManager* colManager, Entity* dialogBox_)
{
	auto npc = mngr_->addEntity();
	auto npctr = npc->addComponent<Transform>();
	npctr->init(Vector2D(sdlutils().width()/2, 360), Vector2D(), 50, 100, 0.0f, false);
	npc->addComponent<Image>(&sdlutils().images().at("matt"));
	auto col = npc->addComponent<RectangleCollider>(npctr->getWidth() + 100, npctr->getHeight() + 100);
	colManager->addCollider(col);
	col->setIsTrigger(true);


	npc->addComponent<NpcCtrl>(colManager, dialogBox_);
}
