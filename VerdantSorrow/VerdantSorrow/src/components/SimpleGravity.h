#pragma once
#include "../ecs/Component.h"
#include "RectangleCollider.h"

#include <vector>
#include <iostream>

class Transform;
class SimpleGravity :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_GRAVITY)
    SimpleGravity(float gravity);

    ~SimpleGravity();
    void initComponent() override;

    void update() override;

    /**
    Método que permite cambiar la variable active, que indica si la gravedad hace efecto
    *
    @param set: Valor que tomará active
    */
    void setActive(bool set);
    /**
    Método que indica si la gravedad se encuentra activada
    *
    @return true si está activada, false en caso contrario
    */
    bool isActive();

protected:
    Transform* tr_;
    float gravity_;

    bool active_;
};

