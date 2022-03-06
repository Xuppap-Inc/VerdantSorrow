#pragma once
#include "../OnBorders.h"

class Transform;
class BossAtributos;
class FramedImage;

class CollideWithBordersBoss :
    public OnBorders
{
public:
    CollideWithBordersBoss();
    ~CollideWithBordersBoss();
    void initComponent() override;

    void onBottom() override;
    void onLeft() override;
    void onRight() override;
    void onNoLeftAndRight() override;
    void onNoTopAndBot() override;

protected:
    BossAtributos* attr_;
    FramedImage* sprite_;
};

