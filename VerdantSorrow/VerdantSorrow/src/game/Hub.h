#pragma once
#include <vector>
#include "CollisionManager.h"
#include "Scene.h"

class TileMap;

class Hub :public Scene
{
public:
	Hub();
	~Hub();
	void init();

	virtual void update() override;
	void playerGenerator(CollisionManager* colManager);
	void createLights();
	bool getAble();
	void setAble(bool a);
	void changeScene_(bool ch);
	void checkCollissions();

	
private:
	CollisionManager* colManager;
	bool isAble;
	bool changeSc;
	Entity* player_;
	Music* musica_;
	TileMap* tileMap_;
};