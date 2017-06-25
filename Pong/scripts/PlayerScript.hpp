/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/Window/Event.hpp>

#include <Core/components/Script.hpp>
#include <Core/state/State.hpp>
#include <Core/ecs/Entity.hpp>
#include <Core/components/EntityTag.hpp>
#include <Core/components/SimpleBody.hpp>
#include <Core/physic/CollisionReaction.hpp>

#include <Pong/Enums.hpp>

namespace con
{
	struct PlayerScriptPong final :
		ScriptComponent
	{
		sf::Keyboard::Key keyUp = sf::Keyboard::Up, keyDown = sf::Keyboard::Down;
		float movementForce = 600.0f;

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

			auto& body = this->entity->GetComponent<SimpleBodyComponent>();

			if ( sf::Keyboard::isKeyPressed( keyUp ) )
				body.velocity.y = -movementForce;
			else if ( sf::Keyboard::isKeyPressed( keyDown ) )
				body.velocity.y = movementForce;
			else
				body.velocity.y = 0;
		}

		void OnCollision( SimpleBodyComponent& first, SimpleBodyComponent& second, collisionSide_t side ) override
		{
			CollisionReaction::CorrectPositionAfterCollision( first, second, side );
			CollisionReaction::StopInCollisionAxis( first, side );
		}
	};

	struct PlayerScriptArkanoid final :
		ScriptComponent
	{
		sf::Keyboard::Key keyLeft = sf::Keyboard::Left, keyRight = sf::Keyboard::Right;
		float movementForce = 600.0f;

		void Update() override
		{
			if ( !this->context->window->hasFocus() )
				return;

			auto& body = this->entity->GetComponent<SimpleBodyComponent>();

			if ( sf::Keyboard::isKeyPressed( keyLeft ) )
				body.velocity.x = -movementForce;
			else if ( sf::Keyboard::isKeyPressed( keyRight ) )
				body.velocity.x = movementForce;
			else
				body.velocity.x = 0;
		}

		void OnCollision( SimpleBodyComponent& first, SimpleBodyComponent& second, collisionSide_t side ) override
		{
			CollisionReaction::CorrectPositionAfterCollision( first, second, side );
			CollisionReaction::StopInCollisionAxis( first, side );
		}
	};
}