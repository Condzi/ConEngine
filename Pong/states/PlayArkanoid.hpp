/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/Audio/Music.hpp>

#include <Core/state/State.hpp>
#include <Core/ecs/EntityManager.hpp>
#include <Core/ecs/EntityFactory.hpp>
#include <Core/components/Drawable.hpp>
#include <Core/components/Position.hpp>

#include <Pong/Enums.hpp>
#include <Pong/EntityCreators.hpp>

namespace con
{
	class PlayArkanoid final :
		public State
	{
	public:
		PlayArkanoid( StateStack& stack, Context& cont ) :
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

			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_PADDLE_ARKANOID, this->context );
			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BALL, this->context );
			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BACKGROUND, this->context );

			auto& topBorder = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BORDER, this->context );
			auto& bottomBorder = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BORDER, this->context );
			auto& leftBorder = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BORDER, this->context );;
			auto& rightBorder = entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_BORDER, this->context );;

			topBorder.GetComponent<SimpleBodyComponent>().position.y = -20.0f;
			topBorder.GetComponent<SimpleBodyComponent>().bb.size.Set( static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_X" ) ), 20.0f );
			bottomBorder.GetComponent<SimpleBodyComponent>().position.y = static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_Y" ) );
			bottomBorder.GetComponent<SimpleBodyComponent>().bb.size.Set( static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_X" ) ), 20.0f );

			leftBorder.GetComponent<SimpleBodyComponent>().position.x = -20.0f;
			leftBorder.GetComponent<SimpleBodyComponent>().bb.size.Set( 20.0f, static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_Y" ) ) );
			rightBorder.GetComponent<SimpleBodyComponent>().position.x = static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_X" ) );
			rightBorder.GetComponent<SimpleBodyComponent>().bb.size.Set( 20.0f, static_cast<float>( settings->GetInt( "WINDOW", "DESIGNED_Y" ) ) );

			auto& cache = *this->context.resourceCache;
			cache.uiTexts.emplace_back( std::make_unique<uiTextResource_t>( RESOURCE_MULTISTATE, TEXT_POINTS ) );
			auto& pointsText = cache.uiTexts.back();
			pointsText->setString( "0:0" );
			pointsText->setFont( *cache.GetFont( FONT_DEFAULT ) );

			entityFactory->CreateEntity( entityManager->CreateEntity(), ENTITY_UI_POINTS_TEXT, this->context ).GetComponent<DrawableTextScript>().textToDraw = pointsText.get();
			int musicNumber = Random::value( 1, 2 );
			std::string finalMusicName = "music" + std::to_string( musicNumber ) + ".ogg";
			// TODO: Add this to future more-elastic default settings class or something like that (music path and volume)
			if ( !this->music.openFromFile( finalMusicName ) )
				LOG( "Cannot open music file: " << finalMusicName, ERROR, BOTH );
			else
			{
				this->music.setVolume( static_cast<float>( settings->GetInt( "SOUND", "VOLUME" ) ) );
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
					entityGroup[0]->GetComponent<SimpleBodyComponent>().position.Set(realMousePos.x, realMousePos.y);
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
		Vec2<uint8_t> points = { 0,0 };
	};
}