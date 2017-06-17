/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/components/Script.hpp>
#include <Core/components/Position.hpp>
#include <Core/components/Velocity.hpp>
#include <Core/components/Drawable.hpp>
#include <Core/state/State.hpp>
#include <Core/ecs/Entity.hpp>
#include <Core/Random.hpp>
#include <Core/Settings.hpp>

namespace con
{
	struct BallScript :
		ScriptComponent
	{
		float maxSpeed = 1000.0f;
		float startSpeed = 600.0f;
		float acceleration = 0.5f;
		Time changeColorTime = asSeconds( 0.6f );
		Time changeColorAccumulator = 0;

		void Update() override
		{
			// Disabled because caused physic acceleration issues (it should be updated in physic using UPS).
			//auto& velocity = this->entity->GetComponent<VelocityComponent>();

			//if ( std::fabsf( velocity.x ) < maxSpeed )
			//{
			//	if ( velocity.x > 0 )
			//		velocity.x += acceleration;
			//	else
			//		velocity.x -= acceleration;
			//}

			auto& sprite = this->entity->GetComponent<DrawableComponent>().sprite;
			auto spriteColor = sprite.getColor();
			this->changeColorAccumulator += Time::FRAME_TIME;
			if ( this->changeColorAccumulator >= this->changeColorTime )
			{
				this->changeColorAccumulator = 0;
				spriteColor.r = static_cast<sf::Uint8>( Random::value( 0, 255 ) );
				spriteColor.g = static_cast<sf::Uint8>( Random::value( 0, 255 ) );
				spriteColor.b = static_cast<sf::Uint8>( Random::value( 0, 255 ) );
				sprite.setColor( spriteColor );
			}
		}

		void OnCollision( SimpleColliderComponent& first, SimpleColliderComponent& second, collisionSide_t side ) override
		{
			LOG( "Ball collision, side: " << +side, INFO, CONSOLE );
			auto& velocity = first.entity->GetComponent<VelocityComponent>();
			first.CorrectAfterCollision( second.boundingBox, side );;
			if ( second.entity->HasComponent<VelocityComponent>() )
				velocity.y += second.entity->GetComponent<VelocityComponent>().y * 0.5f;

			// Bouncing 
			if ( side == COLLISION_SIDE_LEFT || side == COLLISION_SIDE_RIGHT )
				velocity.x = -velocity.x;
			if ( side == COLLISION_SIDE_BOTTOM || side == COLLISION_SIDE_TOP )
				velocity.y = -velocity.y;
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