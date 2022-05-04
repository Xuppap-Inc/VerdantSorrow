#pragma once

#include <vector>
#include "BaseMenu.h"

class Image;
class ParticleSystem;
class MenuScene : public BaseMenu
{
private:
	//Vector con los nombres de los botones aignados en "menu.json"
	std::vector<std::string> buttonNames_= {  "new_Game","continue","settings","controls","credits","quit" };
	std::vector<Transform *> buttonPositions_; //Vector que guarda los transforms de los botones
	std::vector<Entity*> buttonPoperties_; //Vector que guarda los botones como entidad (para el cambio de alfa)
	int mouseIndex_;
	int controllerIndex_;
	float delay_; //Delay del mando
	float lastUpdate_;

	bool buttonsActive_;

	ecs::Entity* background_;
	ecs::Entity* background1_;
	ecs::Entity* background2_;

	Image* background1Img_;
	Image* background2Img_;

	ParticleSystem* particlesys_;
	ParticleSystem* particlesys2_;

	Music* musica_;
	void onButtonClicked(int index)override; //Versión raton
	void generateAllButtons() override;
	void background();
	int musicVolume_;
public:
	MenuScene();
	~MenuScene() {};
	
	void update()override;
	
	void init() override;
	Music* getMusic() { return musica_; };
	int* getMusicVolume() { return &musicVolume_; };
};

