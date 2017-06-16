/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/ecs/EntityFactory.hpp>
#include <Core/state/State.hpp>
#include <Core/resourceManaging/ResourceHolder.hpp>
#include <Core/components/Drawable.hpp>
#include <Core/components/Position.hpp>
#include <Core/components/EntityTag.hpp>
#include <Core/components/SimpleCollider.hpp>
#include <Core/state/State.hpp>
#include <Core/scripts/DrawableTextScript.hpp>

#include <Pong/Enums.hpp>
#include <Pong/scripts/PlayerScript.hpp>
#include <Pong/scripts/BallScript.hpp>
#include <Pong/scripts/TriggerScript.hpp>
#include <Pong/scripts/ExitButtonScript.hpp>

namespace con
{
	struct PaddleACreator :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_PADDLE_A;
		}

		void CreateEntity( Entity& entity, Context& context ) override
		{
			auto& drawable = entity.AddComponent<DrawableComponent>();
			auto& collider = entity.AddComponent<SimpleColliderComponent>();
			auto& position = entity.AddComponent<PositionComponent>();
			entity.AddComponent<VelocityComponent>();
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_PADDLE_A;
			entity.AddScriptComponent<PlayerScript>( context );

			drawable.drawLayer = LAYER_PADDLE;
			drawable.sprite.setTexture( *context.resourceCache->GetTexture( TEXTURE_SHEET ) );
			drawable.sprite.setTextureRect( { 4,0,4,32 } );
			drawable.sprite.setScale( 5.0f, 5.0f );

			collider.boundingBox.size.x = drawable.sprite.getGlobalBounds().width;
			collider.boundingBox.size.y = drawable.sprite.getGlobalBounds().height;
			collider.lockXaxis = true;

			position.x = context.settings->GetInt( "WINDOW", "DESIGNED_X" ) * 0.05f;
			position.y = context.settings->GetInt( "WINDOW", "DESIGNED_Y" ) * 0.5f - collider.boundingBox.size.y * 0.5f;

			entity.AddGroup( GROUP_PLAY_STATE );
		}
	};

	struct PaddleBCreator :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_PADDLE_B;
		}

		void CreateEntity( Entity& entity, Context& context ) override
		{
			auto& drawable = entity.AddComponent<DrawableComponent>();
			auto& collider = entity.AddComponent<SimpleColliderComponent>();
			auto& position = entity.AddComponent<PositionComponent>();
			entity.AddComponent<VelocityComponent>();
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_PADDLE_B;
			entity.AddScriptComponent<PlayerScript>( context );

			drawable.drawLayer = LAYER_PADDLE;
			drawable.sprite.setTexture( *context.resourceCache->GetTexture( TEXTURE_SHEET ) );
			drawable.sprite.setTextureRect( { 4,0,4,32 } );
			drawable.sprite.setScale( 5.0f, 5.0f );

			collider.boundingBox.size.x = drawable.sprite.getGlobalBounds().width;
			collider.boundingBox.size.y = drawable.sprite.getGlobalBounds().height;
			collider.lockXaxis = true;

			position.x = context.settings->GetInt( "WINDOW", "DESIGNED_X" ) * 0.95f;
			position.y = context.settings->GetInt( "WINDOW", "DESIGNED_Y" ) * 0.5f - collider.boundingBox.size.y * 0.5f;

			entity.AddGroup( GROUP_PLAY_STATE );
		}
	};

	struct BorderCreator :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_BORDER;
		}

		void CreateEntity( Entity& entity, Context& ) override
		{
			entity.AddComponent<SimpleColliderComponent>();
			entity.AddComponent<PositionComponent>();
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_BORDER;
			entity.AddGroup( GROUP_PLAY_STATE );
		}
	};

	struct TriggerCreator :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_TRIGGER;
		}

		void CreateEntity( Entity& entity, Context& context ) override
		{
			entity.AddComponent<SimpleColliderComponent>().isTrigger = true;
			entity.AddComponent<PositionComponent>();
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_TRIGGER;
			entity.AddScriptComponent<TriggerScript>( context );
			entity.AddGroup( GROUP_PLAY_STATE );
		}
	};

	struct BallCreator :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_BALL;
		}

		void CreateEntity( Entity& entity, Context& context ) override
		{
			auto& drawable = entity.AddComponent<DrawableComponent>();
			auto& collider = entity.AddComponent<SimpleColliderComponent>();
			entity.AddComponent<VelocityComponent>();
			entity.AddComponent<PositionComponent>();
			entity.AddScriptComponent<BallScript>( context ).ResetBall( Random::value( 0, 1 ) );
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_BALL;
			entity.AddGroup( GROUP_PLAY_STATE );

			drawable.drawLayer = LAYER_BALL;
			drawable.sprite.setTexture( *context.resourceCache->GetTexture( TEXTURE_SHEET ) );
			drawable.sprite.setTextureRect( { 8,28,4,4 } );
			drawable.sprite.setScale( 5.0f, 5.0f );

			collider.boundingBox.size = { drawable.sprite.getGlobalBounds().width, drawable.sprite.getGlobalBounds().height };
		}
	};

	struct UIPointsCreator final :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_UI_POINTS_TEXT;
		}

		void CreateEntity( Entity& entity, Context& context )
		{
			entity.AddComponent<PositionComponent>().x = context.settings->GetInt( "WINDOW", "DESIGNED_X" ) / 2.0f;
			entity.AddComponent<DrawableComponent>().drawLayer = LAYER_UI;
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_UI_POINTS_TEXT;
			entity.AddScriptComponent<DrawableTextScript>( context );
			entity.AddGroup( GROUP_PLAY_STATE );
		}
	};

	struct UIPauseCreator final :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_UI_PAUSE_TEXT;
		}

		void CreateEntity( Entity& entity, Context& context )
		{
			auto& position = entity.AddComponent<PositionComponent>();
			position.x = context.settings->GetInt( "WINDOW", "DESIGNED_X" ) / 2.0f;
			position.y = context.settings->GetInt( "WINDOW", "DESIGNED_Y" ) / 2.0f;

			entity.AddComponent<DrawableComponent>().drawLayer = LAYER_UI;
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_UI_PAUSE_TEXT;
			entity.AddScriptComponent<DrawableTextScript>( context );
			entity.AddGroup( GROUP_PAUSE_STATE );
		}
	};

	struct UIExitButtonCreator final :
		EntityCreator
	{
		entityID_t GetID() const override
		{
			return ENTITY_UI_EXIT_BUTTON;
		}

		void CreateEntity( Entity& entity, Context& context )
		{
			auto& position = entity.AddComponent<PositionComponent>();
			position.x = context.settings->GetInt( "WINDOW", "DESIGNED_X" ) / 2.0f;
			position.y = context.settings->GetInt( "WINDOW", "DESIGNED_Y" ) / 1.5f;
			auto& drawable = entity.AddComponent<DrawableComponent>();
			entity.AddComponent<EntityTagComponent>().tag = ENTITY_UI_EXIT_BUTTON;
			entity.AddScriptComponent<ExitButtonScript>( context );

			drawable.drawLayer = LAYER_UI;
			drawable.sprite.setTexture( *context.resourceCache->GetTexture( TEXTURE_SHEET ) );
			drawable.sprite.setTextureRect( { 8,0,56,28 } );
			drawable.sprite.setScale( 5.0f, 5.0f );
		}
	};
}