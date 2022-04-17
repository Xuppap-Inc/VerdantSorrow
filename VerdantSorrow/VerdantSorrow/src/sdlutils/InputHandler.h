// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>
#include <vector>
#include "../utils/Vector2D.h"

#include "../utils/Singleton.h"

using namespace std;

// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

class InputHandler: public Singleton<InputHandler> {

	friend Singleton<InputHandler> ;

public:
	enum MOUSEBUTTON : uint8_t {
		LEFT = 0, MIDDLE = 1, RIGHT = 2
	};

	virtual ~InputHandler() {
	}

	// clear the state
	inline void clearState() {
		isKeyDownEvent_ = false;
		isKeyUpEvent_ = false;
		isMouseButtonEvent_ = false;
		isMouseMotionEvent_ = false;

		// Mando
		isButtonDownEvent_ = false;
		isButtonUpEvent_ = false;
		isAxisMotionEvent_ = false;

		isQuitEvent_ = false;
		for (auto i = 0u; i < 3; i++) {
			mbState_[i] = false;
		}
	}

	int t;

	// update the state with a new event
	inline void update(const SDL_Event &event) {

		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;

			// CONTROLLER
		case SDL_JOYAXISMOTION:
			onAxisMotion(event);
			break;
		case SDL_JOYBUTTONDOWN:
			onButtonDown(event);
			break;
		case SDL_JOYBUTTONUP:
			onButtonUp(event);
			break;
		case SDL_CONTROLLERDEVICEADDED:
			InitController(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			//onControllerRemoved(event);
			break;

		case SDL_QUIT:
			isQuitEvent_ = true;
		default:
			break;
		}

	
	}

	// CONTROLLER

	// Buttons
	inline void InitController(int id) {

		// Si se conecta un segundo mando ignorarlo
		if (controller) return;

		SDL_GameController* c = SDL_GameControllerOpen(id);
		controller = c;

		// Crea los espacios para las teclas
		vector<int> aux;
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
			aux.push_back(0);
		buttonStates_ = aux;
	}

	inline bool controllerConnected() { return controller; }
	
	inline bool controllerDownEvent() {
		return isButtonDownEvent_;
	}

	inline bool controllerUpEvent() {
		return isButtonUpEvent_;
	}

	inline bool isControllerButtonDown(SDL_GameControllerButton button) {
		if (!controller) return false;
		return (isButtonDownEvent_ && buttonStates_[button]);
	}

	inline bool isControllerButtonUp(SDL_GameControllerButton button) {
		if (!controller) return false;
		return (isButtonUpEvent_ && !buttonStates_[button]);
	}

	inline void onButtonDown(const SDL_Event& event) {

		isButtonDownEvent_ = true;

		Uint8 i = 0;
		bool found = false;
		//int id = sysToGameId.find(event.cdevice.which)->second;

		// Encontramos el bot�n
		while (!found && i < SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX) {
			SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForButton(controller, (SDL_GameControllerButton)i);
			if (b.value.button == event.cbutton.button)
				found = true;
			else
				i++;
		}

		if (found)
			buttonStates_[i] = true;
	}

	inline void onButtonUp(const SDL_Event& event) {

		isButtonUpEvent_ = true;

		Uint8 i = 0;
		bool found = false;
		//int id = sysToGameId.find(event.cdevice.which)->second;

		// Encontramos el bot�n
		while (!found && i < SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX) {
			SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForButton(controller, (SDL_GameControllerButton)i);
			if (b.value.button == event.cbutton.button)
				found = true;
			else
				i++;
		}

		if (found)
			buttonStates_[i] = false;
	}

	// Joysticks
	inline bool isAxisMotionEvent() {
		return isAxisMotionEvent_;
	}

	inline float getAxisValue(SDL_GameControllerAxis axis) {
		if (!controller)
			return 0.0f;

		switch (axis)
		{
		case SDL_CONTROLLER_AXIS_LEFTX:
			return leftJoystick_.getX();
			break;
		case SDL_CONTROLLER_AXIS_LEFTY:
			return leftJoystick_.getY();
			break;
		case SDL_CONTROLLER_AXIS_RIGHTX:
			return rightJoystick_.getX();
			break;
		case SDL_CONTROLLER_AXIS_RIGHTY:
			return rightJoystick_.getY();
			break;
		default:
			break;
		}
	}

	inline void onAxisMotion(const SDL_Event& event) {

		isAxisMotionEvent_ = true;

		Uint8 i = 0;
		bool found = false;
		int axis;

		// Encontramos el eje que se ha movido
		while (!found && i < SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX) {
			SDL_GameControllerButtonBind b = SDL_GameControllerGetBindForAxis(controller, (SDL_GameControllerAxis)i);
			if (b.value.axis == event.jaxis.axis) {
				axis = i;
				found = true;
			}
			++i;
		}

		float value;
		// Si es joystick
		if (axis < 4) {
			if (event.jaxis.value > 10000 || event.jaxis.value < -10000)
				value = event.jaxis.value / 32768.0;
			else
				value = 0;

			switch (axis) {
			case 0:
				leftJoystick_.setX(value);
				break;
			case 1:
				leftJoystick_.setY(value);
				break;
			case 2:
				rightJoystick_.setX(value);
				break;
			case 3:
				rightJoystick_.setY(value);
				break;
			}
		}
		// Si es gatillo
		else {
			//if (event.jaxis.value > 10000)
			//	value = abs(event.jaxis.value);
			//else
			//	value = 0;
			//switch (axis) {
			//case 4:
			//	leftTriggers_[controllerID] = value;
			//	break;
			//case 5:
			//	rightTriggers_[controllerID] = value;
			//	break;
			//}
		}
	}



	// TECLADO

	// refresh
	inline void refresh() {
		SDL_Event event;

		clearState();
		while (SDL_PollEvent(&event))
			update(event);
	}

	// keyboard
	inline bool keyDownEvent() {
		return isKeyDownEvent_;
	}

	inline bool keyUpEvent() {
		return isKeyUpEvent_;
	}

	inline bool isKeyDown(SDL_Scancode key) {
		return keyDownEvent() && kbState_[key] == 1;
	}

	inline bool isKeyDown(SDL_Keycode key) {
		return isKeyDown(SDL_GetScancodeFromKey(key));
	}

	inline bool isKeyUp(SDL_Scancode key) {
		return keyUpEvent() && kbState_[key] == 0;
	}

	inline bool isKeyUp(SDL_Keycode key) {
		return isKeyUp(SDL_GetScancodeFromKey(key));
	}

	// mouse
	inline bool mouseMotionEvent() {
		return isMouseMotionEvent_;
	}

	inline bool mouseButtonEvent() {
		return isMouseButtonEvent_;
	}

	inline const std::pair<Sint32, Sint32>& getMousePos() {
		return mousePos_;
	}

	inline int getMouseButtonState(MOUSEBUTTON b) {
		return mbState_[b];
	}

	inline bool isQuitPressed() {
		return isQuitEvent_;
	}
	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'

private:
	InputHandler() {
		kbState_ = SDL_GetKeyboardState(0);
		clearState();
	}

	inline void onKeyDown(const SDL_Event&) {
		isKeyDownEvent_ = true;
	}

	inline void onKeyUp(const SDL_Event&) {
		isKeyUpEvent_ = true;
	}

	inline void onMouseMotion(const SDL_Event &event) {
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;

	}

	inline void onMouseButtonChange(const SDL_Event &event, bool isDown) {
		isMouseButtonEvent_ = true;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

	bool isQuitEvent_;
	// Keyboard
	bool isKeyUpEvent_;
	bool isKeyDownEvent_;
	bool isMouseMotionEvent_;
	bool isMouseButtonEvent_;

	// Controller
	SDL_GameController* controller;
	bool isButtonDownEvent_;
	bool isButtonUpEvent_;
	bool isAxisMotionEvent_;

	Vector2D leftJoystick_ = { 0, 0 };
	Vector2D rightJoystick_ = { 0, 0 };

	std::pair<Sint32, Sint32> mousePos_;
	std::array<bool, 3> mbState_;
	const Uint8 *kbState_;

	// Almacena todos los estados de todos los botones
	vector<int> buttonStates_;
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}
