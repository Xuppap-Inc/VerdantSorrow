#pragma once
#include "../../ecs/Component.h"
#include "root/RootMovement.h"

class Transform;
class CollisionManager;
class BossAtributos;
class FramedImage;

class RootSpawner : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTSPAWNER)

	RootSpawner(CollisionManager* collManager);
	~RootSpawner();

	void initComponent() override;
	void createRoot(int x);

	int getRootWidth() { return rootWidth_; };

private:
	CollisionManager* collManager_;
	FramedImage* framedImg_;

	int rootWidth_;
};

