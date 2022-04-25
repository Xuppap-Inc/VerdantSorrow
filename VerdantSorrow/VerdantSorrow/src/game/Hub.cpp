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
#include "../components/ScrollCamera.h"

#include "CollisionManager.h"
#include "../game/SceneManager.h"
#include "../components/fondos/ParticleSystem.h"
#include "../components/fondos/Light.h"
#include "../components/hub/PlatformAtribsForHub.h"
#include "../sdlutils/Texture.h"
#include "../json/JSONValue.h"
#include "TileMap.h"




Hub::Hub() :Scene()
{
	colManager = nullptr;
}

Hub::~Hub()
{
	delete tileMap;
}

void Hub::init()
{
	Scene::init();

	//Para gestionar las colisiones
	colManager = new CollisionManager();
	mngr_->setColManager(colManager);

	tileMap = new TileMap(mngr_, "resources/hub/mapa.tmx",colManager);


	changeSc = false;
	//backgroundHub();
	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGenerator(colManager, player);
	auto camera = mngr_->addEntity();
	auto cameraTr = camera->addComponent<Transform>();
	cameraTr->init(Vector2D(0, 0), Vector2D(0, 0), 0, 0, 0);
	auto cameraC = camera->addComponent<ScrollCamera>(3);
	mngr_->setHandler(ecs::_hdlr_CAMERA, camera);
	//Genera las entradas a los bosses
		//entrada a la rana
	EntryGenerator(entryFrog, colManager, 0, 250, "frog");
	//Entrada al ojo
	EntryGenerator(entryTree, colManager, sdlutils().width() - 100, 100, "tree");
	//Entrada al arbol
	EntryGenerator(entryEye, colManager, sdlutils().width() - 100, sdlutils().height() - 100, "eye");

	auto dialogBox = mngr_->addEntity();
	dialogBoxGenerator(dialogBox);
	NPCGenerator(colManager, dialogBox);

	musica_ = &sdlutils().musicsHub().at("musica_hub");
	musica_->play();
	musica_->setMusicVolume(60);

	auto hoguera = mngr_->addEntity();
	auto hogueraTr = hoguera->addComponent<Transform>();
	hogueraTr->init(Vector2D(100, 300), Vector2D(), 10, 20, 0.0f);
	hogueraTr->setScale(0.25);
	hoguera->addComponent<FramedImage>(&sdlutils().imagesHub().at("spritesheet_hoguera"), 6, 6, (1000 / 30) * 34, 34, "spritesheet_hoguera");

	createLights();
	hoguera->addToGroup(ecs::_HUB_DECORATION_GRP);
}

void Hub::dialogBoxGenerator(Entity* dialogBox)
{
	dialogBox->setActive(false);
	auto tr = dialogBox->addComponent<Transform>();
	tr->init(Vector2D((sdlutils().width() - 600) / 2, (sdlutils().height() - 200)), Vector2D(), 600, 150, 0.0f, false);
	dialogBox->addComponent<DialogBoxMngr>("PTMONO24");
	dialogBox->addToGroup(ecs::_UI_GRP);
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
		Entity* entWithName = new Entity();
		while (!changeScene && i < colliders.size()) {
			changeScene = colliders[i]->isActive() && colliders[i]->isTrigger() && colliders[i]->getEntity()->getComponent<NpcCtrl>() == nullptr;
			if (changeScene) entWithName = colliders[i]->getEntity();
			i++;
		}
		if (changeScene){
			if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "frog") sC().FrogSceneState(true);
			else if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "tree") sC().TreeSceneState(true);
			else if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "eye") sC().EyeSceneState(true);
			changeScene_(true);
		}
	}
}

void Hub::backgroundHub()
{
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().imagesHub().at("fondoHub"));
	backgr_->addToGroup(ecs::_BACKGROUND_1_GRP);
}


void Hub::update()
{
	if (!changeSc) {
		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		
		mngr_->render();
#ifdef _DEBUG
		mngr_->debug();
#endif
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
	auto playerX = sdlutils().width() / 2 ;
	auto playerY = sdlutils().height() / 2 ;
	//Se le dan las posiciones iniciales, vecocidad, ancho y alto al player
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 200, 200, 0.0f, 0.25f, false);
	player_->addComponent<FramedImage>(&sdlutils().imagesHub().at("idle_Kyna"), 4, 8, (1000 / 30) * 30, 30, "idle_Kyna");
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	//Se a�ade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	colManager->addCollider(playerCollider);
	//Componente que permite controlar al jugador
	player_->addComponent<PlayerHubControl>(2, colManager);

	
	//Componente ui jugador
	mngr_->setHandler(ecs::_PLAYER, player_);
	auto* playerLife_ = mngr_->addEntity();
	playerLife_->addComponent<PlayerUI>(true);
	playerLife_->addToGroup(ecs::_UI_GRP);

	player_->addToGroup(ecs::_PLAYER_GRP);
}

void Hub::EntryGenerator(Entity* entry, CollisionManager* colManager, float posX, float posY, std::string name)
{
	entry = mngr_->addEntity();

	auto entryTr = entry->addComponent<Transform>();
	auto entryX = 0;
	auto entryY = sdlutils().height() / 4 * 3;
	entryTr->init(Vector2D(posX, posY), Vector2D(), 100, 100, 0.0f);

	entry->addComponent<RectangleRenderer>();

	auto entryCollider = entry->addComponent<RectangleCollider>(entryTr->getWidth(), entryTr->getHeight());
	colManager->addCollider(entryCollider);
	entryCollider->setIsTrigger(true);
	entry->addComponent<PlatformAtribsForHub>(name);
	entry->addToGroup(ecs::_HUB_DECORATION_GRP);
}

void Hub::NPCGenerator(CollisionManager* colManager, Entity* dialogBox_)
{
	auto npc = mngr_->addEntity();
	auto npctr = npc->addComponent<Transform>();
	npctr->init(Vector2D(sdlutils().width() / 2, 360), Vector2D(), 50, 100, 0.0f, false);
	npc->addComponent<Image>(&sdlutils().imagesHub().at("matt"));
	auto col = npc->addComponent<RectangleCollider>(npctr->getWidth() + 100, npctr->getHeight() + 100);
	colManager->addCollider(col);
	col->setIsTrigger(true);
	npc->addComponent<NpcCtrl>(colManager, dialogBox_);


	npc->addToGroup(ecs::_HUB_DECORATION_GRP);
}

void Hub::createLights() {
	new Light(&sdlutils().imagesHub().at("luz_naranja"), -200, -100, 800, 100, mngr_);
}