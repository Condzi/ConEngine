/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/components/SimpleBody.hpp>

namespace con
{
	namespace CollisionReaction
	{
		void CorrectPositionAfterCollision( SimpleBodyComponent& first, const SimpleBodyComponent& second, collisionSide_t side );
		void StopInCollisionAxis( SimpleBodyComponent& body, collisionSide_t side );
	}
}