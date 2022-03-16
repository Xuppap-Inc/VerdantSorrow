#include "LanternMovement.h"
#include "TreeMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../components/Transform.h"

LanternMovement::LanternMovement() :treeTr_(), lanternTr_()
{
}

LanternMovement::LanternMovement(Transform* treeTransform_)
	:treeTr_(treeTransform_),
	lanternTr_()
{
}

LanternMovement::~LanternMovement()
{
}

void LanternMovement::initComponent()
{
	 lanternTr_ = ent_->getComponent<Transform>();
	assert(lanternTr_ != nullptr);


}

void LanternMovement::update()
{
	lanternTr_->getPos() = treeTr_->getPos();
	std::cout << "lantern tr: " << lanternTr_ << std::endl;
	std::cout << "tree tr: " << treeTr_ << std::endl;
}
