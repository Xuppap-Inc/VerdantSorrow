#pragma once

#include "../../ecs/Component.h"
#include "PlayerAttributes.h"

class Texture;

class PlayerUI : public ecs::Component{
public:
        __CMPID_DECL__(ecs::_PLAYERUI)
        PlayerUI(Texture* heart_);
        virtual ~PlayerUI();

        void initComponent() override;
        void render() override;

protected:
    Texture* heart;
    Texture* heartBlack;
    PlayerAttributes* attrib_;

    Texture* healthDetailTexture;
};

