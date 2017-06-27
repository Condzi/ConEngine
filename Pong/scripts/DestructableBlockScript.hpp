/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once
#include <Core/components/Script.hpp>
#include <Core/components/Position.hpp>
#include <Core/components/EntityTag.hpp>
#include <Core/components/SimpleBody.hpp>
#include <Core/state/State.hpp>
#include <Core/ecs/EntityManager.hpp>
#include <Core/ecs/Functions.hpp>
#include <Core/Settings.hpp>

#include <Pong/Enums.hpp>
#include <Pong/scripts/BallScript.hpp>

namespace con
{
	struct DestructableBlockScript final :
		ScriptComponent
	{
		uint8_t durability = 1;
		// How many times ball does hit this block?
		uint8_t hits = 0;

		void OnCollision( SimpleBodyComponent& first, SimpleBodyComponent& second, collisionSide_t side ) override
		{
			CON_UNUSED_PARAM( first );
			CON_UNUSED_PARAM( second );
			CON_UNUSED_PARAM( side );

			// Implement scoring here.
			this->hits++;
			if ( this->hits >= this->durability )
				this->entity->Kill();
		}
	};
}