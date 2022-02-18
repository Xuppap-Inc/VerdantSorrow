#pragma once
#include "../OnBorders.h"

class Transform;

class CollideWithBordersBoss :
    public OnBorders
{
public:
    CollideWithBordersBoss();
    ~CollideWithBordersBoss();

    void onBottom() override;
    void onLeft() override;
    void onRight() override;

protected:
};

