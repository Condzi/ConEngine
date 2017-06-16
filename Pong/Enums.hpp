/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <cinttypes>

#include <Core/Config.hpp>

namespace con
{
	enum state_t : stateID_t
	{
		STATE_EXIT = EXIT_STATE,
		STATE_DEBUG_DATA = DEBUG_DATA_STATE,
		STATE_LOADING_SCREEN,
		STATE_MENU,
		STATE_PLAY,
		STATE_PAUSE
	};

	enum system_t : systemID_t
	{
		// Core systems, don't touch!
		SYSTEM_POSITION_UPDATE = 0, // Updates position of Drawables, Collider to match PositionComponent
		SYSTEM_SIMPLE_PHYSIC, // Update ball and collisions with 'score zones' and colliders.
		SYSTEM_SCRIPT_UPDATE,
		SYSTEM_RENDERER,
		// Pong systems.
	//	SYSTEM_SOUND, // Maybe Core system? Plays requested sound from resourceCache.
		SYSTEM_EVENT_UPDATE, // Updates Components that needs to check if X key is pressed. (sets bool IsKey-X-Pressed to true or false)
		SYSTEM_PADDLE_UPDATE, // If key_up pressed the paddle go up etc... Associated with EVENT_UPDATE (same Components)
	};

	enum group_t : groupID_t
	{
		GROUP_MENU_STATE = 0,
		GROUP_PLAY_STATE,
		GROUP_PAUSE_STATE
	};

	enum resources_t : resourceID_t
	{
		TEXTURE_SHEET = 0,
		MUSIC,
		FONT_DEFAULT,

		TEXT_POINTS,
		TEXT_PAUSE
	};

	enum resourcePriority_t : resourcePriorityID_t
	{
		RESOURCE_MULTISTATE = 0
	};

	enum entity_t : uint8_t
	{
		ENTITY_PADDLE_A = 0,
		ENTITY_PADDLE_B,
		ENTITY_BALL,
		ENTITY_BORDER,
		ENTITY_TRIGGER,

		ENTITY_UI_POINTS_TEXT,
		ENTITY_UI_PAUSE_TEXT
	};

	enum drawLayer_t : int8_t
	{
		LAYER_BACKGROUND = 0,
		LAYER_PADDLE,
		LAYER_BALL,
		LAYER_UI
	};
}