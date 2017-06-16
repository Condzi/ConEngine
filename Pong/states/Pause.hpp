/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/state/State.hpp>
#include <Core/systems/Renderer.hpp>
#include <Core/time/Time.hpp>

#include <Pong/Enums.hpp>
#include <Pong/EntityCreators.hpp>

namespace con
{
	class PauseState final :
		public State
	{
	public:
		PauseState( StateStack& stack, Context& cont ) :
			State( stack, cont )
		{}

		stateID_t GetID() const override { return STATE_PAUSE; }

		void OnPush() override
		{
			this->context.resourceCache->uiTexts.emplace_back( std::make_unique<uiTextResource_t>( RESOURCE_MULTISTATE, TEXT_PAUSE ) );
			auto& textResource = this->context.resourceCache->uiTexts.back();
			textResource->setString( "PAUSE" );
			textResource->setFont( *this->context.resourceCache->GetFont( FONT_DEFAULT ) );
			textResource->setCharacterSize( 60 );

			auto& text = this->context.entityFactory->CreateEntity( this->context.entityManager->CreateEntity(), ENTITY_UI_PAUSE_TEXT, this->context );
			text.GetComponent<DrawableTextScript>().textToDraw = textResource.get();
			// Center text.
			auto &position = text.GetComponent<PositionComponent>();
			// Text height is 1/2 for default for some reason.
			position.y = position.y - textResource->getGlobalBounds().height;
			position.x = position.x - textResource->getGlobalBounds().width / 2.0f;

			auto& exitButton = this->context.entityFactory->CreateEntity( this->context.entityManager->CreateEntity(), ENTITY_UI_EXIT_BUTTON, this->context );
			exitButton.AddGroup( GROUP_PAUSE_STATE );
			auto& exitButtonPos = exitButton.GetComponent<PositionComponent>();
			exitButtonPos.x -= exitButton.GetComponent<DrawableComponent>().sprite.getGlobalBounds().width / 2;
		}

		void OnPop() override
		{
			// IDEA: Maybe just keep track of these entities and deactivate them on pop and activate on push?
			auto entitiesToKill = this->context.entityManager->GetEntitiesByGroup( GROUP_PAUSE_STATE );
			for ( auto entity : entitiesToKill )
				entity->Kill();

			this->context.resourceCache->DeleteAllResourcesByID( TEXT_PAUSE );
		}
		void Update() override
		{
			if ( !this->context.window->hasFocus() )
				return;

			this->pauseTime += Time::FRAME_TIME;
			// Eleminate bug with immediately escaping from pause to game 
			if ( this->pauseTime > asSeconds( 0.5f ) )
				if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
				{
					this->requestStackPop();
					this->pauseTime = 0;
				}
		}

	private:
		Time pauseTime;
	};
}