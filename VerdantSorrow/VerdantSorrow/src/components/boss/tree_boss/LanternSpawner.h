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
	void createLantern(int x, int y);
	void update();


private:
	CollisionManager* colManager_;
	FramedImage* framedImg_;
	Transform* tr_;
	int* posX_;
	int* posY;

	int lanternWidht_;
	
};

