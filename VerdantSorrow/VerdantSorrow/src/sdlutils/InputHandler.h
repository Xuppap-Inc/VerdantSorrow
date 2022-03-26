// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>
#include <vector>

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

		for (auto i = 0u; i < 3; i++) {
			mbState_[i] = false;
		}
	}

	int t;

	// update the state with a new event
	inline void update(const SDL_Event &event) {

		//t++;

		//cout << "update" << t << endl;
		//cout << "event.type = " << event.type << endl;

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

			// Mando
		case SDL_JOYAXISMOTION:
			//onAxisMotion(event);
			//cout << "motion" << endl;
			break;
		case SDL_JOYBUTTONDOWN:
			onButtonDown(event);
			break;
		case SDL_JOYBUTTONUP:
			onButtonUp(event);
			//cout << "up" << endl;
			break;
		case SDL_CONTROLLERDEVICEADDED:
			InitController(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			//onControllerRemoved(event);
			break;
		case SDL_QUIT:
			//isQuitEvent_ = true;
		default:
			break;
		}
	}

	// MANDO
	
	//inline bool isButtonDown(int controller, SDL_GameControllerButton button) {
	//	//if (controllers_[controller] == nullptr)
	//	//	return false;
	//	return (isButtonDownEvent_ && buttonStates_[controller][button]);
	//}

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

		cout << "buttonStates_ = " << endl;
		for (size_t i = 0; i < buttonStates_.size(); i++)
			cout << i << "= " << buttonStates_[i] << endl;
	}

	inline void onButtonUp(const SDL_Event& event) {

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
			buttonStates_[i] = false;

		cout << "buttonStates_ = " << endl;
		for (size_t i = 0; i < buttonStates_.size(); i++)
			cout << i << "= " << buttonStates_[i] << endl;
	}

	//inline bool isButtonDown(int controller, SDL_GameControllerButton button) {
	//	if (!controller) return false;
	//	return (isButtonDownEvent_ && buttonStates_[controller][button]);
	//}


	SDL_GameController* controller;

	inline void InitController(int id) {

		// Si se conecta un segundo mando ignorarlo
		if (controller) return;

		SDL_GameController* c = SDL_GameControllerOpen(id);
		controller = c;
		cout << SDL_GameControllerName(controller) << endl;
		cout << "InitController" << endl;

		// Crea los espacios para las teclas
		vector<int> aux;
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
			aux.push_back(0);
		buttonStates_ = aux;

		cout << "buttonStates_ = " << buttonStates_.size() << endl;
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

	bool isKeyUpEvent_;
	bool isKeyDownEvent_;
	bool isMouseMotionEvent_;
	bool isMouseButtonEvent_;
	bool isButtonDownEvent_;
	bool isButtonUpEvent_;
	bool isAxisMotionEvent_;

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
