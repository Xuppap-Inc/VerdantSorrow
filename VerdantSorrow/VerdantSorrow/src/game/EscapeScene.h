#pragma once

#include <vector>
#include "CollisionManager.h"

#include "Scene.h"
class CollisionChecker;
class TileMap;

class EscapeScene : public Scene
{
public:
	EscapeScene();
	~EscapeScene();
	void init();

	void update()override;

	//start lo hace scene directamente
	void background();


	bool getAble();
	void setAble(bool a);
	

	int* getMusicVolume() { return &musicVolume_; };
	Music* getMusic() { return musica_; };

	void createBoss(CollisionManager* col);
private:
	bool isAble;
	Entity* player;
	Entity* FinalBossFace;
	CollisionChecker* colCheck_;
	TileMap* tileMap_;
	void changescenes();
	int musicVolume_;
	Music* musica_;
};

