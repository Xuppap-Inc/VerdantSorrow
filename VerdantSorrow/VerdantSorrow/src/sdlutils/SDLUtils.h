// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL.h>
#include <string>
#include <map>

#include "../utils/Singleton.h"
#include "RandomNumberGenerator.h"
#include "Font.h"
#include "Music.h"
#include "SoundEffect.h"
#include "Texture.h"
#include "VirtualTimer.h"


class SDLUtils: public Singleton<SDLUtils> {

	friend Singleton<SDLUtils> ; // needed to give access to private constructors

public:
	void setWindowSize(int w, int h){
		width_ = w;
		height_ = h;
		SDL_SetWindowSize(window_, w, h);
		SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	// we abstract away the actual data structure we use for
	// tables. All we assume is that is has the following
	// methods
	//
	//   emplace(key,value)
	//   at(key)
	//   clear()
	//
	template<typename T>
	using sdl_resource_table = std::map<std::string,T>;

	virtual ~SDLUtils();

	// cannot copy/move
	SDLUtils(SDLUtils&) = delete;
	SDLUtils(SDLUtils&&) = delete;
	SDLUtils& operator=(SDLUtils&) = delete;
	SDLUtils& operator=(SDLUtils&&) = delete;

	// access to the underlying SDL_Window -- in principle not needed
	inline SDL_Window* window() {
		return window_;
	}

	// access to the underlying SDL_Renderer -- needed when creating textures
	// other than those initialized in this class
	inline SDL_Renderer* renderer() {
		return renderer_;
	}

	// clear the renderer with a given SDL_Color
	inline void clearRenderer(SDL_Color bg = build_sdlcolor(0xAAAAAFF)) {
		SDL_SetRenderDrawColor(renderer_, COLOREXP(bg));
		SDL_RenderClear(renderer_);
	}

	// present the current content of the renderer
	inline void presentRenderer() {
		SDL_RenderPresent(renderer_);
	}

	// the window's width
	inline int width() {
		return 1920;
	}

	inline int windowWidth() {
		return width_;
	}

	// the window's height
	inline int height() {
		return 1080;
	}

	inline int windowHeight() {
		return height_;
	}
	// toggle to full-screen/window mode
	inline void toggleFullScreen() {
		auto flags = SDL_GetWindowFlags(window_);
		if (flags & SDL_WINDOW_FULLSCREEN) {
			SDL_SetWindowFullscreen(window_, 0);
		} else {
			SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
		}
	}

	// show the cursor when mouse is over the window
	inline void showCursor() {
		SDL_ShowCursor(1);
	}

	// hide the cursor when mouse is over the window
	inline void hideCursor() {
		SDL_ShowCursor(0);
	}

	// All resource maps can be modified from outside, this way you can store
	// your own dynamically. Be careful when modifying them!

	// fonts map
	inline sdl_resource_table<Font>& fonts() {
		return fonts_;
	}

	inline sdl_resource_table<Font>& fontsHub() {
		return fonts_hub;
	}

	// images map
	inline sdl_resource_table<Texture>& images() {
		return images_;
	}

	inline sdl_resource_table<Texture>& imagesHub() {
		return images_hub;
	}

	// sound effects map
	inline sdl_resource_table<SoundEffect>& soundEffects() {
		return sounds_;
	}

	inline sdl_resource_table<SoundEffect>& soundEffectsHub() {
		return sounds_hub;
	}

	// musics maps
	inline sdl_resource_table<Music>& musics() {
		return musics_;
	}

	inline sdl_resource_table<Music>& musicsHub() {
		return musics_hub;
	}

	//dialogs maps
	inline sdl_resource_table<std::string>& dialogs() {
		return dialogs_;
	}

	// Access to the random number generator. It is important to always
	// use this generator, this way you can regenerate the same sequence
	// if you start from the same seed
	inline RandomNumberGenerator& rand() {
		return random_;
	}

	// Access to the virtual timer, it is useful when you allow to 'pause'
	// your game, also for synchronising clocks of players (when using sdlnet)
	inline VirtualTimer& virtualTimer() {
		return timer_;
	}

	// Access to real time
	inline Uint32 currRealTime() const {
		return SDL_GetTicks();
	}
	
	void freeMemory();
	void loadReasources(std::string filename); // load resources from the json file
	void loadReasourcesHub(std::string filename); // load resources from the json file
	void initWindow();
	void closeWindow();
	void setWidth(int w) { width_ = w; };
	void setHeight(int h) { height_ = h; };
private:
	SDLUtils();
	SDLUtils(std::string windowTitle, int width, int height);
	SDLUtils(std::string windowTitle, int width, int height,
			std::string filename);

	void initSDLExtensions(); // initialize resources (fonts, textures, audio, etc.)
	void closeSDLExtensions(); // free resources the

	std::string windowTitle_; // window title
	int width_; // window width
	int height_; // window height

	SDL_Window *window_; // the window
	SDL_Renderer *renderer_; // the renderer

	sdl_resource_table<Font> fonts_hub; // fonts map (string -> font)
	sdl_resource_table<Texture> images_hub; // textures map (string -> texture)
	sdl_resource_table<SoundEffect> sounds_hub; // sounds map (string -> sound)
	sdl_resource_table<Music> musics_hub; // musics map (string -> music)

	sdl_resource_table<Font> fonts_; // fonts map (string -> font)
	sdl_resource_table<Texture> images_; // textures map (string -> texture)
	sdl_resource_table<SoundEffect> sounds_; // sounds map (string -> sound)
	sdl_resource_table<Music> musics_; // musics map (string -> music)

	sdl_resource_table<std::string> dialogs_; // musics map (string -> string)

	RandomNumberGenerator random_; // (pseudo) random numbers generator
	VirtualTimer timer_; // virtual timer

	int assetsNumber_;
	void renderProgressBar(int assetsLoaded);

	Texture* logoCargando_;
};


// This macro defines a compact way for using the singleton SDLUtils, instead of
// writing SDLUtils::instance()->method() we write sdlutils().method()
//
inline SDLUtils& sdlutils() {
	return *SDLUtils::instance();
}
