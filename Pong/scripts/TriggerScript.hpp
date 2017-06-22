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
	struct TriggerScript final :
		ScriptComponent
	{
		sf::Vector2<uint8_t>* points = nullptr;

		void OnCollision( SimpleBodyComponent& first, SimpleBodyComponent& second, collisionSide_t side ) override
		{
			CON_UNUSED_PARAM( first );
			if ( second.entity->GetComponent<EntityTagComponent>().tag != ENTITY_BALL )
				return;

			CON_ASSERT( side == COLLISION_SIDE_LEFT || side == COLLISION_SIDE_RIGHT, "wrong collision side with trigger" );
			CON_ASSERT( this->points, "points pointer is nullptr for unknow reason" );

			if ( side == COLLISION_SIDE_LEFT )
			{
				this->points->y++;
				second.entity->GetComponent<BallScript>().ResetBall( false );
			} else if ( side == COLLISION_SIDE_RIGHT )
			{
				this->points->x++;
				second.entity->GetComponent<BallScript>().ResetBall( true );
			}

			LOG( "Score! " << +this->points->x << " : " << +this->points->y, INFO, CONSOLE );
		}
	};
}