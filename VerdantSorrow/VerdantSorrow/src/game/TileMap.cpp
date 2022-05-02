#include "TileMap.h"
#include "../sdlutils/SDLUtils.h"
#include "../json/JSON.h"
#include "../components/Transform.h"
#include "../components/RectangleCollider.h"
#include "../components/Image.h"
#include "../components/hub/NpcCtrl.h"
#include "../components/hub/DialogBoxMngr.h"
#include "../components/hub/PlatformAtribsForHub.h"
#include "../game/Game.h"
#include "../components/hub/HubAreas.h"
#include "../components/boss/nivelHuida/Mushroom.h"
#include "../components/boss/nivelHuida/FlySpawnerPlant.h"

TileMap::TileMap(ecs::Manager* mngr, string tileMapPath, CollisionManager* col, double scale, Pivot pivot) :col_(col), dialogBox_(nullptr), scale_(scale), pivot_(pivot)
{
	path = tileMapPath;
	rows = cols = tileWidth = tileHeight = 0;
	mngr_ = mngr;
	dialogBox_ = mngr_->addEntity();
	dialogBoxGenerator(dialogBox_);
	loadMap(path);
}

TileMap::~TileMap()
{
	if (tmxTileMap != nullptr)
		delete tmxTileMap;

	if (tileMap != nullptr)
		SDL_DestroyTexture(tileMap);
}


void TileMap::loadMap(string path)
{
	tmxTileMap = new tmx::Map();
	tmxTileMap->load(path);

	// tamaño del mapa
	auto mapDimensions = tmxTileMap->getTileCount();
	rows = mapDimensions.y;
	cols = mapDimensions.x;

	// tamaño de los tiles
	auto tileSize = tmxTileMap->getTileSize();
	tileWidth = tileSize.x;
	tileHeight = tileSize.y;


	tileMap = SDL_CreateTexture(sdlutils().renderer(),

		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		tileWidth * cols,
		tileHeight * rows
	);
	SDL_SetTextureBlendMode(tileMap, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(sdlutils().renderer(), tileMap);


	loadTilesetsTextures();

	createObjects();

	SDL_SetRenderTarget(sdlutils().renderer(), nullptr);

}

void TileMap::createObjects()
{
	auto mapWidth = tileWidth * cols;
	auto mapHeight = tileHeight * rows;

	// recorremos cada una de las capas (de momento solo las de tiles) del mapa
	auto& map_layers = tmxTileMap->getLayers();

	for (auto& layer : map_layers) {
		// aqui comprobamos que sea la capa de tiles
		if (layer->getType() == tmx::Layer::Type::Object) {

			// cargamos la capa
			tmx::ObjectGroup* objects = dynamic_cast<tmx::ObjectGroup*>(layer.get());

			// obtenemos sus tiles
			for (auto& object : objects->getObjects()) {

				auto pos = object.getPosition();
				auto size = object.getAABB();
				SDL_Rect r = build_sdlrect(pos.x, pos.y, size.width, size.height);

				auto cur_gid = object.getTileID();
				string name = objects->getName();

				if (cur_gid != 0) { // si es distinto de 0 tiene sprite

					//el pivote de los objetos esta abajo
					r.y -= size.height;

					// el mas cercano, y a la vez menor, al gid del tile)
					auto tset_gid = -1;
					for (auto& ts : tilesets) {
						if (ts.first <= cur_gid) {
							tset_gid = ts.first;
						}
						else
							break;
					}

					// si no hay tileset v�lido, continuamos a la siguiente iteracion
					if (tset_gid == -1)
						continue;

					// normalizamos el �ndice
					cur_gid -= tset_gid;

					//render con respecto a la textura
					if (name != "entradasbosses")
						tilesets[tset_gid][cur_gid]->render(r);

				}

				//trasformacion de objetos teniendo en cuenta la escala y su pivote (para añadirlos a escena)
				r.x = r.x / scale_;
				r.w = r.w / scale_;
				r.h = r.h / scale_;

				if (pivot_ == BOTTONLEFT)
					r.y = (r.y - mapHeight) / scale_ + sdlutils().height();
				else if (pivot_ == TOPLEFT)
					r.y = r.y / scale_;

				//add entidades correspondientes
				if (name == "colliders" || name == "entradasbosses" || name == "npc" || name == "areas"||name=="Pinchos" || name=="LanzaMoscas" || name == "Setas") {

					ecs::Entity* ent = mngr_->addEntity();
					auto tr = ent->addComponent<Transform>();
					tr->init(Vector2D(r.x, r.y), Vector2D(), r.w, r.h, 0.0f);
					auto col = ent->addComponent<RectangleCollider>(r.w, r.h);
					col_->addCollider(col);

					if (name == "npc") {

						col->setIsTrigger(true);
						auto npcctrl = ent->addComponent<NpcCtrl>(col_, dialogBox_);

						vector<tmx::Property> properties = object.getProperties();
						int i = 0;
						while (i < properties.size() && properties[i].getName() != "npc")i++;

						if (i < properties.size())
							npcctrl->setDialog(sdlutils().dialogs().at("npc" + to_string(properties[i].getIntValue()) + "_dialogue" + to_string((int)Game::instance()->state_)));

						ent->addToGroup(ecs::_HUB_DECORATION_GRP);
					}
					else if (name == "entradasbosses") {
						vector<tmx::Property> properties = object.getProperties();

						int i = 0;
						while (i < properties.size() && properties[i].getName() != "Boss")i++;

						if (i < properties.size()) {
							ent->addComponent<Image>(&sdlutils().imagesHub().at(properties[i].getStringValue() + "_" + "open"));
							ent->addComponent<PlatformAtribsForHub>(properties[i].getStringValue());
						}
						ent->addToGroup(ecs::_UI_GRP);
					}
					else if (name == "areas") {
						col->setIsTrigger(true);


						vector<tmx::Property> properties = object.getProperties();

						int i = 0;
						while (i < properties.size() && properties[i].getName() != "area")i++;

						if (i < properties.size())
							ent->addComponent<HubAreas>(col_, properties[i].getStringValue());

						ent->addToGroup(ecs::_UI_GRP);
					}
					else if (name == "Setas") {
						ent->addComponent<Mushroom>(col_);
						ent->addToGroup(ecs::_BOSSELEMENTS_GRP);
					}
					else if (name == "LanzaMoscas") {
						ent->addComponent<FlySpawnerPlant>(col_, true, 5000);
						ent->addToGroup(ecs::_BOSSELEMENTS_GRP);
					}
					else if (name == "Pinchos") {
						col->setIsTrigger(true);
					}

				}
			}
		}
	}

	//add map as entity (teniendo en cuenta el pivote y la escala)
	auto map = mngr_->addEntity();

	auto tr = map->addComponent<Transform>();
	Vector2D pos = Vector2D(0, pivot_ == TOPLEFT ? 0 : sdlutils().height() - mapHeight / scale_);
	tr->init(pos, Vector2D(), mapWidth / scale_, mapHeight / scale_, 0.0f);

	map->addComponent<Image>(new Texture(sdlutils().renderer(), tileMap, tileWidth * cols, tileHeight * rows));
	map->addToGroup(ecs::_HUB_DECORATION_GRP);
	mngr_->setHandler(ecs::_hdlr_TILEMAP, map);
}

void TileMap::loadTilesetsTextures()
{
	auto& mapTilesets = tmxTileMap->getTilesets();
	for (auto& tileset : mapTilesets) {

		auto tilesetId = tileset.getFirstGID();

		for (auto& sprite : tileset.getTiles()) {

			Uint imgId = sprite.ID;
			string imagePath = sprite.imagePath;
			tilesets[tilesetId].insert(pair<Uint, Texture*>(imgId, new Texture(sdlutils().renderer(), imagePath)));
		}
	}
}

void TileMap::dialogBoxGenerator(ecs::Entity* dialogBox)
{
	dialogBox->setActive(false);
	auto tr = dialogBox->addComponent<Transform>();

	tr->init(Vector2D((sdlutils().width() - 600) / 2, (sdlutils().height() - 200)), Vector2D(), 600, 150, 0.0f, false);
	dialogBox->addComponent<DialogBoxMngr>("PTMONO24");
	dialogBox->addToGroup(ecs::_UI_GRP);
}