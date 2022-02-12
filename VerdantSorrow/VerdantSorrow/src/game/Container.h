#pragma once
#include "GameObject.h"
#include <vector>

class InputComponent;
class RenderComponent;
class PhysicsComponent;

class Container : public GameObject
{
public:
	Container();
	virtual ~Container();

	void handleInput() override;
	void update() override;
	void render() override;

	inline void addComponent(PhysicsComponent* pc) {
		pc_.push_back(pc);
	}

	inline void addComponent(InputComponent* ic) {
		ic_.push_back(ic);
	}

	inline void addComponent(RenderComponent* rc) {
		rc_.push_back(rc);
	}
private:
	std::vector<InputComponent*> ic_;
	std::vector<PhysicsComponent*> pc_;
	std::vector<RenderComponent*> rc_;
};

