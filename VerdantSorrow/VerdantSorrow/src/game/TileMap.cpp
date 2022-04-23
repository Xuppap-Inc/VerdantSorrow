#include "TileMap.h"
#include "../sdlutils/SDLUtils.h"
#include "../json/JSON.h"
#include "../components/Transform.h"
#include "../components/RectangleCollider.h"
#include "../components/Image.h"

TileMap::TileMap(ecs::Manager* mngr, string tileMapPath)
{
	path = tileMapPath;
	rows = cols = tileWidth = tileHeight = 0;
	mngr_ = mngr;
	scaleX = scaleY = 0.5;

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

	// recorremos cada una de las capas (de momento solo las de tiles) del mapa
	auto& map_layers = tmxTileMap->getLayers();
	for (auto& layer : map_layers) {
		// aqui comprobamos que sea la capa de tiles
		if (layer->getType() == tmx::Layer::Type::Object) {
			// cargamos la capa
			tmx::ObjectGroup* objects = dynamic_cast<tmx::ObjectGroup*>(layer.get());

			// obtenemos sus tiles

			for (auto& object : objects->getObjects()) {
				auto cur_gid = object.getTileID();

				if (cur_gid == 0)
					continue;

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

				auto pos = object.getPosition();
				SDL_Rect r = build_sdlrect(pos.x, pos.y - object.getAABB().height, object.getAABB().width, object.getAABB().height);
				tilesets[tset_gid][cur_gid]->render(r);

				if (objects->getName() != "suelo" && objects->getName() != "camino") {
					auto tileMapWidth = tileWidth * cols;
					auto tileMapHeight = tileHeight * rows;

					double dx = sdlutils().windowWidth() / (double)tileMapWidth;
					double dy = sdlutils().windowHeight() / (double)tileMapHeight;

					dx /= scaleX;
					dy /= scaleY;

					ecs::Entity* ent = mngr_->addEntity();
					auto tr = ent->addComponent<Transform>();
					tr->init(Vector2D(r.x * dx, r.y * dy), Vector2D(), r.w * dx, r.h * dy, 0.0f);
					ent->addComponent<RectangleCollider>(r.w * dx, r.h * dy);
				}
			}
		}
	}
	SDL_SetRenderTarget(sdlutils().renderer(), nullptr);

	//add map as entity
	auto map = mngr_->addEntity();
	auto tr = map->addComponent<Transform>();
	tr->init(Vector2D(), Vector2D(), sdlutils().windowWidth() / scaleX, sdlutils().windowHeight() / scaleY, 0.0f);
	map->addComponent<Image>(new Texture(sdlutils().renderer(), tileMap, tileWidth * cols, tileHeight * rows));
	map->addToGroup(ecs::_HUB_DECORATION_GRP);
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
