#pragma once

#include "../../ecs/Component.h"
#include "PlayerAttributes.h"

class Texture;

class PlayerUI : public ecs::Component{
public:
        __CMPID_DECL__(ecs::_PLAYERUI)
        PlayerUI(Texture* tex);
        virtual ~PlayerUI();

        void initComponent() override;
        void render() override;

protected:
    Texture* tex_;
    PlayerAttributes* attrib_;

    Texture* healthDetailTexture;
};

