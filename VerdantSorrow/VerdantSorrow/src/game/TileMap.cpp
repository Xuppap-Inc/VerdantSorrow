#include "TileMap.h"
#include "../sdlutils/SDLUtils.h"
#include "../json/JSON.h"
#include "../components/Transform.h"

TileMap::TileMap(ecs::Manager* mngr, string jsonPath, string tileMapPath)
{
	path = tileMapPath;
	jsPath = jsonPath;
	rows = cols = tileWidth = tileHeight = 0;
	mngr_ = mngr;

	loadMap(path);
}

TileMap::~TileMap()
{
	if (tmxTileMap != nullptr)
		delete tmxTileMap;
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

				auto aabb = object.getAABB();
				tilesets[tset_gid][cur_gid]->render(build_sdlrect(aabb.left,aabb.top,aabb.width,aabb.height));

			}
		}
	}

	SDL_SetRenderTarget(sdlutils().renderer(), nullptr);

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

void TileMap::render()
{
	auto b = mngr_->getHandler(ecs::_hdlr_CAMERA);
	auto a = b->getComponent<Transform>();
	// Dibujado del mapa
	SDL_Rect dest = { 0,0, sdlutils().width(), sdlutils().height() };

	SDL_Rect src = { a->getPos().getX(), a->getPos().getY(),
			 sdlutils().width() * 0.5,
			 sdlutils().height() * 0.5};

	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	dest.x *= sW;
	dest.w *= sW;
	dest.y *= sH;
	dest.h *= sH;
	src.x *= sW;
	src.w *= sW;
	src.y *= sH;
	src.h *= sH;

	SDL_RenderCopy(sdlutils().renderer(), tileMap, &src, &dest);
}
