#pragma once
#include "../../OnBorders.h"

class Transform;
class BossAtributos;
class FramedImage;

class BounceOnBordersEye :
    public OnBorders
{
public:
    BounceOnBordersEye();
    ~BounceOnBordersEye();
    void initComponent() override;

    void onBottom() override;
    void onTop() override;
    void onLeft() override;
    void onRight() override;
    void onNoLeftAndRight() override;
    void onNoTopAndBot() override;

protected:
    BossAtributos* attr_;
    FramedImage* sprite_;
};

