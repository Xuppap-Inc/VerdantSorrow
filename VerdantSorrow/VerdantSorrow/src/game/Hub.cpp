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

#include "CollisionManager.h"



void Hub::init()
{
	Scene::init();

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();


	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	EntryGenerator(colManager);
	auto dialogBox = mngr_->addEntity();
	dialogBoxGenerator(dialogBox);
	NPCGenerator(colManager, dialogBox);
}

void Hub::dialogBoxGenerator(Entity* dialogBox)
{
	dialogBox->setActive(false);
	auto tr = dialogBox->addComponent<Transform>();
	tr->init(Vector2D((sdlutils().width() - 600) / 2, (sdlutils().height() - 200)), Vector2D(), 600, 150, 0.0f, false);
	dialogBox->addComponent<RectangleRenderer>();
	dialogBox->addComponent<DialogBoxMngr>("ARIAL24");
}

bool Hub::getAble()
{
	return isAble;
}

void Hub::setAble(bool a)
{
	isAble = a;
}


void Hub::update()
{
	mngr_->update();
	mngr_->refresh();

	sdlutils().clearRenderer();
	mngr_->render();
	mngr_->debug();
	sdlutils().presentRenderer();
}

void Hub::playerGenerator(CollisionManager* colManager, Entity* player_) {
	//Se le añaden los atributos del player, no los del transform
	player_->addComponent<PlayerAttributes>();
	//Se le añade el transform
	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	//Se le dan las posiciones iniciales, vecocidad, ancho y alto al player
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 50, 100, 0.0f, false);

	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	player_->addComponent<CollideWithBorders>();
	colManager->addCollider(playerCollider);
	//Componente que permite controlar al jugador
	player_->addComponent<PlayerHubControl>(3, colManager);

	//No poner estas físicas detrás del playerctrl, se hunde y no funciona el salto
	//player_->addComponent<SimplePhysicsPlayer>(colManager);
	player_->addComponent<Image>(&sdlutils().images().at("chica"));


	//Componente ui jugador
	player_->addComponent<PlayerUI>();
}

void Hub::EntryGenerator(CollisionManager* colManager)
{
	auto frogEntry = mngr_->addEntity();

	auto frogEntryTr = frogEntry->addComponent<Transform>();
	auto frogEntryX = sdlutils().width() / 3 - 200;
	auto frogEntryY = sdlutils().height() / 4 * 3;
	frogEntryTr->init(Vector2D(frogEntryX, frogEntryY), Vector2D(), 200, 50, 0.0f);

	frogEntry->addComponent<RectangleRenderer>();

	auto frogEntryCollider = frogEntry->addComponent<RectangleCollider>(frogEntryTr->getWidth(), frogEntryTr->getHeight());
	colManager->addCollider(frogEntryCollider);
	frogEntryCollider->setIsTrigger(true);
}

void Hub::NPCGenerator(CollisionManager* colManager, Entity* dialogBox_)
{
	auto npc = mngr_->addEntity();
	auto npctr = npc->addComponent<Transform>();
	npctr->init(Vector2D(800, 400), Vector2D(), 50, 100, 0.0f, false);
	npc->addComponent<Image>(&sdlutils().images().at("matt"));
	auto col = npc->addComponent<RectangleCollider>(npctr->getWidth() + 100, npctr->getHeight() + 100);
	colManager->addCollider(col);
	col->setIsTrigger(true);

	npc->addComponent<RectangleRenderer>();

	npc->addComponent<NpcCtrl>(colManager, dialogBox_);
}
