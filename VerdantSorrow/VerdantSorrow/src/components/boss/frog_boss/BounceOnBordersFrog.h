#pragma once
#include "../../OnBorders.h"

class Transform;
class BossAtributos;
class FramedImage;

class BounceOnBordersFrog :
    public OnBorders
{
public:
    BounceOnBordersFrog();
    ~BounceOnBordersFrog();
    void initComponent() override;

    void onBottom() override;
    void onLeft() override;
    void onRight() override;
    void onNoTopAndBot() override;

protected:
    BossAtributos* attr_;
    FramedImage* sprite_;
};

