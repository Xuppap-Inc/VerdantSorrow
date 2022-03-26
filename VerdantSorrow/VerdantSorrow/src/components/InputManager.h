//#pragma once
//#include <vector>
//#include "../components/RectangleCollider.h"
//
//class InputManager : public Component {
//public:
//	InputManager(int state) :
//		//states_(nullptr),
//		//menu_(nullptr),
//		//stateNumber_(state)
//	{
//	}
//
//	virtual ~InputManager() {}
//
//	void init() override;
//	void update() override;
//
//	void setMousePos(const Vector2D& mousePos) { lastMouseX_ = mousePos.getX(); lastMouseY_ = mousePos.getY(); };
//	Vector2D getMousePos() { return Vector2D(lastMouseX_, lastMouseY_); };
//
//private:
//	//const enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
//	//const enum KEYS2 { UP, DOWN, LEFT, RIGHT, SPACE };
//	//GameStates* states_;
//	//MenuButtonManager* menu_;
//
//	const enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE, BACK };
//
//	std::map<KEYS, SDL_Scancode> keymap;
//
//	//bool handleKeyboard();
//	bool handleController(int controller);
//	//bool handleMouse();
//
//	//bool mouseInButton(float x, float y, SDL_Rect const& button);
//
//	//void back();
//
//
//	int stateNumber_, lastMouseX_ = 0, lastMouseY_ = 0;
//	//char lastHandled_ = ' ';
//};
//
