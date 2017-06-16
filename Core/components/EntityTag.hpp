/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/ecs/Component.hpp>

namespace con
{
	struct EntityTagComponent final :
		Component
	{
		uint8_t tag = 0;
	};
}