
#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/VirtualTimer.h"

class Transform;
class CollisionManager;
class BossAtributos;
class FramedImage;
class VirtualTimer;

class TutorialSpawnRoot : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_TUTORIAL_ROOTSPAWNER)

	TutorialSpawnRoot(CollisionManager* collManager);
	~TutorialSpawnRoot();

	void initComponent() override;
	void update();
	void createRoot(int x);

private:
	CollisionManager* colManager_;
	FramedImage* framedImg_;

	int rootWidth_;
	int rootPos_;
	VirtualTimer* lastTime_;
};

