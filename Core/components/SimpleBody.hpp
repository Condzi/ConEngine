/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/ecs/Component.hpp>
#include <Core/physic/BoundingBox.hpp>

namespace con
{
	struct SimpleBodyComponent final :
		Component
	{
		BoundingBox bb;
		Vec2f position = Vec2f::Zero;
		Vec2f velocity = Vec2f::Zero;
		Vec2f force = Vec2f::Zero;
		Vec2f gravityScale = Vec2f::One;
		float mass = 1.0f;
		bool lockXaxis = false, lockYaxis = false;
	};
}