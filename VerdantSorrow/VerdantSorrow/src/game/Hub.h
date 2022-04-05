#pragma once
#include <vector>
#include "CollisionManager.h"
#include "Scene.h"
class Hub :public Scene
{
public:
	Hub();
	~Hub();
	void init();

	virtual void update() override;
	void playerGenerator(CollisionManager* colManager, Entity* player_);
	void EntryGenerator(Entity* entry, CollisionManager* colManager, float posX, float posY, std::string name);
	void NPCGenerator(CollisionManager* colManager, Entity* dialogBox_);
	void createLights();
	void dialogBoxGenerator(Entity* dialogBox);
	bool getAble();
	void setAble(bool a);
	void changeScene_(bool ch);
	void checkCollissions();
	void backgroundHub();
private:
	CollisionManager* colManager;
	bool isAble;
	bool changeSc;
	Entity* player;
	Entity* entryFrog;
	Entity* entryTree;
	Entity* entryEye;

	Music* musica_;
};