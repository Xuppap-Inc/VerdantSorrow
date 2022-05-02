#pragma once
#include <vector>
#include "Scene.h"
class BaseMenu : public Scene
{
	//No pongo virtual timers porque no nos interesa pausar el propio menu de pausa
protected: 
	bool changeSc_;
public:
	BaseMenu();
	~BaseMenu() {};
	virtual void init()=0; 
	void background(std::string backgroundName);
	void createButton(float x, float y, float w, float h, std::string buttImg, std::vector<Transform*>&buttPos, std::vector<Entity*>&buttProps);
	virtual void update()=0; //Los menus pueden hacer cosas difrentes en el update
	virtual void onButtonClicked(int index)=0; //Virtual puro cada menu hace cosas diferentes segun sus botones
	virtual void generateAllButtons()=0; //Virtual puro, cada menu dibuja sus botones de diferentes formas
	void handleInput(std::vector<Transform*>buttPos, float delay, float &lastUpdate, int &controllerIndex, std::vector<std::string>buttNames,std::vector<Entity*>buttProps);
	void handleMouseInput(std::vector<Transform*>buttPos, std::vector<Entity*>buttProps);
	void handleControllerInput(float delay, float& lastUpdate, int &controllerIndex, std::vector<std::string>buttNames, std::vector<Entity*>buttProps);
	void changeButton(int &controllerIndex, std::vector<std::string>buttNames, float &lastUpdate, int numMoves);
	void selectButton(Entity*buttProps);
	void deselectButton(Entity*buttProps);
	void isChangingScene(bool valor) { changeSc_ = valor; }

};

