#pragma once
#include "../OnBorders.h"

#include "PlayerAttributes.h"

class Transform;

class CollideWithBorders :
    public OnBorders
{
public:
    CollideWithBorders();
    CollideWithBorders(float downOffset);
    ~CollideWithBorders();
    void initComponent() override;

    void onBottom() override;
    void onLeft() override;
    void onRight() override;
    void onTop() override;
    void onNoLeftAndRight() override;
    

protected:

    PlayerAttributes* attrib_;

    // Cambia el limite inferior con el que se collisiona
    float downOffset;
};

