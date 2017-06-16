/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/Audio/Music.hpp>

#include <Core/state/State.hpp>
#include <Core/ecs/EntityManager.hpp>
#include <Core/components/Drawable.hpp>
#include <Core/components/Position.hpp>

#include <Pong/Enums.hpp>
#include <Pong/EntityCreators.hpp>

namespace con
{
	class PlayState final :
		public State
	{
	public:
		PlayState( StateStack& stack, Context& cont ) :
			State( stack, cont )
		{}

		stateID_t GetID() const override
		{
			return STATE_PLAY;
		}

		void OnPush()
		{
			// Create paddles and ball.
			auto entityFactory = this->context.entityFactory;
			auto entityManager = this->context.entityManager;
			auto settings = this->context.settings;

			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_PADDLE_A, this->context );
			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_PADDLE_B, this->context );
			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BALL, this->context );

			auto& topBorder = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BORDER, this->context );
			auto& bottomBorder = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BORDER, this->context );
			auto& triggerLeft = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_TRIGGER, this->context );;
			auto& triggerRight = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_TRIGGER, this->context );;

			topBorder.GetComponent<PositionComponent>().y = -20.0f;
			topBorder.GetComponent<SimpleColliderComponent>().boundingBox.size = { static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_X" ) ), 20.0f };
			bottomBorder.GetComponent<PositionComponent>().y = static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_Y" ) );
			bottomBorder.GetComponent<SimpleColliderComponent>().boundingBox.size = { static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_X" ) ), 20.0f };

			triggerLeft.GetComponent<PositionComponent>().x = -20.0f;
			triggerLeft.GetComponent<SimpleColliderComponent>().boundingBox.size = { 20.0f, static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_Y" ) ) };
			triggerRight.GetComponent<PositionComponent>().x = static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_X" ) );
			triggerRight.GetComponent<SimpleColliderComponent>().boundingBox.size = { 20.0f, static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_Y" ) ) };

			triggerLeft.GetComponent<TriggerScript>().points = &this->points;
			triggerRight.GetComponent<TriggerScript>().points = &this->points;

			// Move it to DrawableText entity.
			auto& cache = *this->context.resourceCache;
			cache.uiTexts.emplace_back( std::make_unique<uiTextResource_t>( RESOURCE_MULTISTATE, TEXT_POINTS ) );
			auto& pointsText = cache.uiTexts.back();
			pointsText->setString( "0:0" );
			pointsText->setFont( *cache.GetFont( FONT_DEFAULT ) );

			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_UI_POINTS_TEXT, this->context ).GetComponent<DrawableTextScript>().textToDraw = pointsText.get();
			// IDEA: Maybe move this to separate method?
			int musicNumber = Random::value( 1, 2 );
			std::string finalMusicName = "music" + std::to_string( musicNumber ) + ".ogg";
			// TODO: Add this to future more-elastic default settings class or something like that (music path and volume)
			if ( !this->music.openFromFile( finalMusicName ) )
				LOG( "Cannot open music file: " << finalMusicName, ERROR, BOTH );
			else
			{
				this->music.setVolume( 30.0f );
				this->music.play();
			}
		}

		void OnPop()
		{
			auto entitiesToKill = this->context.entityManager->GetEntitiesByGroup( GROUP_PLAY_STATE );
			for ( auto entity : entitiesToKill )
				entity->Kill();

			this->music.stop();
		}

		void Update() override
		{
			bool isOnTop = this->imStateOnTopOfTheStack();
			if ( !isOnTop )
				this->music.pause();
			else if ( this->music.getStatus() != sf::Music::Playing )
				this->music.play();

			auto& entityGroup = this->context.entityManager->GetEntitiesByGroup( GROUP_PLAY_STATE );
			for ( auto entity : entityGroup )
				entity->SetActive( isOnTop );
			if ( !isOnTop )
				return;

			auto pointsText = this->context.resourceCache->GetText( TEXT_POINTS );
			pointsText->setString( std::to_string( +points.x ) + " | " + std::to_string( +points.y ) );

			// IDEA: Figure out better way to lock input if window dosesn't have focus.
			if ( !this->context.window->hasFocus() )
				return;

			this->pauseTime += Time::FRAME_TIME;
			if ( this->context.settings->GetBool( "DEBUG", "DEBUG_PROPERTIES" ) )
				if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) )
				{
					auto realMousePos = this->context.window->mapPixelToCoords( sf::Mouse::getPosition( *this->context.window ) );
					entityGroup[0]->GetComponent<PositionComponent>().x = realMousePos.x;
					entityGroup[0]->GetComponent<PositionComponent>().y = realMousePos.y;
				}

			// Eliminate pause bug.
			// IMPORTANT: Figure out how to do it better.
			if ( this->pauseTime > asSeconds( 0.5f ) )
				if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
				{
					this->requestStackPush( STATE_PAUSE );
					this->pauseTime = 0;
				}
		}

	private:
		Time pauseTime;
		sf::Music music;
		sf::Vector2<uint8_t> points = { 0,0 };
	};
}