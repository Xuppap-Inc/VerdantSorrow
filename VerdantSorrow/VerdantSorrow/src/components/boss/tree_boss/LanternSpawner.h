#pragma once

#include "../../../ecs/Component.h"

class Transform;
class CollisionManager;
class BossAtributos;
class FramedImage;

class LanternSpawner: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_LANTERNSPAWNER)

		LanternSpawner(CollisionManager* collManager);
		~LanternSpawner();

	void initComponent() override;
	void createLantern(int x, int y,bool quieta);
	virtual void update();


private:
	CollisionManager* colManager_;
	FramedImage* framedImg_;
	Transform* tr_;
	bool* quieta_;
	int* posX_;
	int* posY;

	int lanternWidht_;
	
};

