//#include "InputManager.h"
//#include "../sdlutils/SDLUtils.h"
//
//#include "../sdlutils/InputHandler.h"
//#include "../ecs/Entity.h"
//
//void InputManager::init() {
//
//	//menu_ = entity_->getComponent<MenuButtonManager>();
//
//	//states_ = entity_->getMngr()->getHandler<StateMachine>()->getComponent<GameStates>();
//	//assert(states_ != nullptr);
//
//	keymap.insert({ UP, SDL_SCANCODE_UP });
//	keymap.insert({ DOWN, SDL_SCANCODE_DOWN });
//	keymap.insert({ LEFT, SDL_SCANCODE_LEFT });
//	keymap.insert({ RIGHT, SDL_SCANCODE_RIGHT });
//	keymap.insert({ SPACE, SDL_SCANCODE_RETURN });
//	keymap.insert({ BACK, SDL_SCANCODE_BACKSPACE });
//}
//
//
////el update recibe todos los input de SDL los filtra y envia la respuesta a la clase
//void InputManager::update() {
//	//auto gameState = states_->getState();
//
//	//if (gameState == stateNumber_) {
//	//	if (gameState != GameStates::HAMSTERSELECTION) {
//	//		bool handled = false;
//	//		int i = 0;
//	//		// Si hay algún mando conectado los tiene en cuenta
//	//		while (!handled && ih().playerHasController(i)) {
//	//			handled = handleController(i);
//	//			i++;
//	//		}
//	//		if (!handled) {
//	//			handleKeyboard();
//	//			handleMouse();
//	//		}
//	//	}
//	//	else {
//	//		int i = sdlutils().hamstersChosen();
//	//		if (ih().playerHasController(i))
//	//			handleController(i);
//	//		else {
//	//			handleKeyboard();
//	//			handleMouse();
//	//		}
//	//	}
//	//}
//
//	handleController(0);
//}
//
//// M�tododos que manejan el input seg�n sea con mando o con teclado
//bool InputManager::handleController(int controller) {
//	bool handled = false;
//
//	if (ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_A)) {
//
//		std::cout << "ButtonPressed" << endl;
//		handled = true;
//	}
//
//	//if (ih().getAxisValue(controller, SDL_CONTROLLER_AXIS_LEFTY) < 0 || ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
//	//	//menu_->moveUp();
//	//	handled = true;
//	//}
//	////DOWN
//	//else if (ih().getAxisValue(controller, SDL_CONTROLLER_AXIS_LEFTY) > 0 || ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
//	//{
//	//	menu_->moveDown();
//	//	handled = true;
//	//}
//
//	////RIGHT
//	//if (ih().getAxisValue(controller, SDL_CONTROLLER_AXIS_LEFTX) > 0 || ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
//	//{
//	//	menu_->moveRight();
//	//	handled = true;
//	//}
//	////	LEFT
//	//else if (ih().getAxisValue(controller, SDL_CONTROLLER_AXIS_LEFTX) < 0 || ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
//	//{
//	//	menu_->moveLeft();
//	//	handled = true;
//	//}
//
//	//if (ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_A)) {
//	//	menu_->pressButton();
//	//	handled = true;
//	//}
//
//	//else if (ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_B)) {
//	//	back();
//	//	handled = true;
//	//}
//
//	return handled;
//	/*auto gameState = states_->getState();
//	if (ih().isButtonDown(controller, SDL_CONTROLLER_BUTTON_START) && gameState == GameStates::PAUSE) {
//		states_->setState(GameStates::RUNNING);
//	}*/
//}
//
