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
		if (layer->getType() == tmx::Layer::Type::Tile) {
			// cargamos la capa
			tmx::TileLayer* tile_layer = dynamic_cast<tmx::TileLayer*>(layer.get());

			// obtenemos sus tiles
			auto& layer_tiles = tile_layer->getTiles();

			// recorremos todos los tiles para obtener su informacion
			for (auto y = 0; y < rows; ++y) {
				for (auto x = 0; x < cols; ++x) {
					// obtenemos el indice relativo del tile en el mapa de tiles
					auto tile_index = x + (y * cols);

					// con dicho indice obtenemos el indice del tile dentro de su tileset
					auto cur_gid = layer_tiles[tile_index].ID;


					// si es 0 esta vacio asi que continuamos a la siguiente iteracion
					if (cur_gid == 0)
						continue;

					// guardamos el tileset que utiliza este tile (nos quedamos con el tileset cuyo gid sea
					// el mas cercano, y a la vez menor, al gid del tile)
					auto tset_gid = -1, tsx_file = 0;;
					for (auto& ts : tilesets) {
						if (ts.first <= cur_gid) {
							tset_gid = ts.first;
							tsx_file++;
						}
						else
							break;
					}

					// si no hay tileset v�lido, continuamos a la siguiente iteracion
					if (tset_gid == -1)
						continue;

					// normalizamos el �ndice
					cur_gid -= tset_gid;

					// calculamos dimensiones del tileset
					auto ts_width = 0;
					auto ts_height = 0;
					SDL_QueryTexture(tilesets[tset_gid]->getTexture(),
						NULL, NULL, &ts_width, &ts_height);

					// calculamos el area del tileset que corresponde al dibujo del tile
					auto region_x = (cur_gid % (ts_width / tileWidth)) * tileWidth;
					auto region_y = (cur_gid / (ts_width / tileHeight)) * tileHeight;

					// calculamos la posicion del tile
					auto x_pos = x * tileWidth;
					auto y_pos = y * tileHeight;


					// metemos el tile
					auto tileTex = tilesets[tset_gid];

					SDL_Rect src;
					src.x = region_x; src.y = region_y;
					src.w = tileWidth;
					src.h = tileHeight;

					SDL_Rect dest;
					dest.x = x_pos;
					dest.y = y_pos;
					dest.w = src.w;
					dest.h = src.h;

					int tileRot = layer_tiles[tile_index].flipFlags;
					float rotCorrection = 45;
					SDL_RendererFlip tileFlip = SDL_FLIP_NONE;


					//Multiplicamos por 45 porque esta multiplicado por factor de 45 (lo que devuelve rot)
					tilesets[tset_gid]->render(src, dest, (double)tileRot * rotCorrection, nullptr, tileFlip);
				}
			}
		}
	}
	SDL_SetRenderTarget(sdlutils().renderer(), nullptr);

}

void TileMap::loadTilesetsTextures()
{
	std::map<string, Texture*> tileSetsMap;
	std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(jsPath));

	// check it was loaded correctly
	// the root must be a JSON object
	if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
		throw "Something went wrong while load/parsing '" + jsPath + "'";
	}

	// we know the root is JSONObject
	JSONObject root = jValueRoot->AsObject();
	JSONValue* jValue = nullptr;


	jValue = root["tilesets"];
	if (jValue != nullptr) {
		if (jValue->IsArray()) {
			for (auto& v : jValue->AsArray()) {
				if (v->IsObject()) {
					JSONObject vObj = v->AsObject();
					std::string key = vObj["id"]->AsString();
					std::string file = vObj["file"]->AsString();

					auto a = new Texture(sdlutils().renderer(), file);
					tileSetsMap.emplace(key, a);
				}
				else {
					throw "'tilesets' array in '" + jsPath
						+ "' includes and invalid value";
				}
			}
		}
		else {
			throw "'tilesets' is not an array";
		}
	}

	auto& mapTilesets = tmxTileMap->getTilesets();
	for (auto& tileset : mapTilesets) {
		string name = tileset.getName();
		Texture* texture = tileSetsMap.find(name)->second;
		tilesets.insert(pair<Uint, Texture*>(tileset.getFirstGID(), texture));
	}
}

void TileMap::render()
{	
	auto b = mngr_->getHandler(ecs::_hdlr_CAMERA);
	auto a = b->getComponent<Transform>();
	// Dibujado del mapa
	SDL_Rect dest = { 0,0, sdlutils().width(), sdlutils().height() };

	SDL_Rect src = {a->getPos().getX(), a->getPos().getY(),
			 sdlutils().width()*0.5,
			 sdlutils().height()*0.5 };

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
