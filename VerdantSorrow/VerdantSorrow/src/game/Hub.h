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
	Music* getMusic() { return musica_; };
	int* getMusicVolume() { return &musicVolume_; };

private:

	void teleportPlayer();

	CollisionManager* colManager;
	bool isAble;
	bool changeSc;
	int musicVolume_;
	Entity* player_;
	Music* musica_;
	TileMap* tileMap_;

	bool firstTimeEnteringGame;

	Vector2D frogDoorPos;
	Vector2D treeDoorPos;
	Vector2D finalDoorPos;
};