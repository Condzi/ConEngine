/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/Window/Event.hpp>

#include <Core/components/Script.hpp>
#include <Core/state/State.hpp>
#include <Core/ecs/Entity.hpp>
#include <Core/components/Velocity.hpp>
#include <Core/components/EntityTag.hpp>

#include <Pong/Enums.hpp>

namespace con
{
	struct PlayerScript final :
		ScriptComponent
	{
		sf::Keyboard::Key keyUp = sf::Keyboard::Up, keyDown = sf::Keyboard::Down;
		float movementForce = 500.0f;

		void Init() override
		{
			auto tag = this->entity->GetComponent<EntityTagComponent>().tag;

			if ( tag == ENTITY_PADDLE_A )
			{
				this->keyUp = sf::Keyboard::W;
				this->keyDown = sf::Keyboard::S;
			}
		}

		void Update() override
		{
			if ( !this->context->window->hasFocus() )
				return;

			auto& velocity = this->entity->GetComponent<VelocityComponent>();
			if ( sf::Keyboard::isKeyPressed( keyUp ) )
				velocity.y = -movementForce;
			else if ( sf::Keyboard::isKeyPressed( keyDown ) )
				velocity.y = movementForce;
			else
				velocity.y = 0;
		}

		void OnCollision( SimpleColliderComponent& first, SimpleColliderComponent& second, collisionSide_t side ) override
		{
			first.CorrectAfterCollision( second.boundingBox, side );
			first.StopInCollisionAxis( side );
		}
	};
}