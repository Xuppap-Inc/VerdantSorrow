#pragma once

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"

#include "./tmxlite/LayerGroup.hpp"
#include "./tmxlite/Object.hpp"

#include <map>

#include <SDL.h>

#include "../ecs/Manager.h"

using Uint = unsigned int;
using namespace std;

class Texture;
class CollisionManager;

class TileMap
{
public:
	TileMap(ecs::Manager* mngr,string tileMapPath, CollisionManager*col);
	~TileMap();
	

private:

	void loadMap(string path);
	void createObjects();
	void loadTilesetsTextures();
	void dialogBoxGenerator(ecs::Entity* dialogBox);
	SDL_Texture* tileMap;
	string path;
	int rows, cols;
	int tileWidth, tileHeight;
	map<Uint, map<Uint,Texture*>> tilesets;//tilemapid -> img id->texture
	tmx::Map* tmxTileMap;
	ecs::Manager* mngr_;
	CollisionManager* col_;
	double scaleX, scaleY;
	ecs::Entity* dialogBox_;
};

