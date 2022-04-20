#pragma once

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"
#include <map>

#include <SDL.h>

#include "../ecs/Manager.h"

using Uint = unsigned int;
using namespace std;

class Texture;

class TileMap
{
public:
	TileMap(ecs::Manager* mngr,string jsonPath,string tileMapPath);
	~TileMap();
	
	void render();

private:

	void loadMap(string path);
	void loadTilesetsTextures();

	SDL_Texture* tileMap;
	string path, jsPath;
	int rows, cols;
	int tileWidth, tileHeight;
	map<Uint, Texture*> tilesets;
	tmx::Map* tmxTileMap;
	ecs::Manager* mngr_;
};

