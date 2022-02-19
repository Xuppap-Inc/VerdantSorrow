#pragma once
#include "../OnBorders.h"

#include "PlayerAttributes.h"

class Transform;

class CollideWithBorders :
    public OnBorders
{
public:
    CollideWithBorders();
    ~CollideWithBorders();
    void initComponent() override;

    void onBottom() override;
    void onLeft() override;
    void onRight() override;
    void onNoLeftAndRight() override;

protected:

    PlayerAttributes* attrib_;
};

