/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/components/Script.hpp>
#include <Core/components/Position.hpp>
#include <Core/components/Velocity.hpp>
#include <Core/state/State.hpp>
#include <Core/ecs/Entity.hpp>
#include <Core/Random.hpp>
#include <Core/Settings.hpp>

namespace con
{
	struct BallScript :
		ScriptComponent
	{
		float maxSpeed = 800.0f;
		float startSpeed = 500.0f;
		float acceleration = 0.5f;

		void Update() override
		{
			auto& velocity = this->entity->GetComponent<VelocityComponent>();

			if ( std::fabsf( velocity.x ) < maxSpeed )
			{
				if ( velocity.x > 0 )
					velocity.x += acceleration;
				else
					velocity.x -= acceleration;
			}
		}

		void ResetBall( bool dirLeft )
		{
			auto& settings = *this->context->settings;
			auto& position = this->entity->GetComponent<PositionComponent>();
			auto& velocity = this->entity->GetComponent<VelocityComponent>();

			position.x = static_cast<float>( settings.GetInt( "WINDOW", "DESIGNED_X" ) * 0.5 );
			position.y = static_cast<float>( settings.GetInt( "WINDOW", "DESIGNED_Y" ) * 0.5 );

			velocity.x = this->startSpeed;
			if ( dirLeft )
				velocity.x = -velocity.x;

			velocity.y = Random::value( -startSpeed * 0.5f, startSpeed * 0.5f );
		}
	};
}