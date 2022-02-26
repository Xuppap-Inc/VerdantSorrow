// This file is part of the course TPV2@UCM - Samir Genaim

// DO NOT USE/MODIFY THIS FILE, IT IS JUST AN EXAMPLE OF HOW YOUR
// game/ecs_defs.h SHOULD LOOK LIKE
//
// Note that the backslash \ after each line below is very important
// when using #define, if you remove it them put all at the same
// line of #define
//

#pragma once

// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	_TRANSFORM, \
	_RENDERER, \
	_ONBORDERS, \
	_PLAYERCTRL, \
	_COLLIDER, \
	_SIMPLEMOVE, \
	_PHYSICS, \
	_ATTRIBUTES, \
	_FROGJUMP, \
	_FROGBIGJUMP, \
	_GRAVITY, \
	_ATTACK,\
	_FRAMEDIMAGE,\
	_IMAGE,\
	_WAVEMOVEMENT, \
	_PLAYERUI, \
	_BOSS_ATTRIBUTES, \
	_BOSS_HP_BAR, \
	_BOSS_ATTACK_MANAGER


// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	_STAR_GRP_, \
	_ASTEROID_GRP

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    _BOSS, \
	_PLAYER

