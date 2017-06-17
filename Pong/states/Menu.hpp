/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/components/Drawable.hpp>
#include <Core/components/Position.hpp>
#include <Core/state/State.hpp>
#include <Core/ecs/EntityManager.hpp>
#include <Core/ecs/EntityFactory.hpp>

#include <Pong/Enums.hpp>

namespace con
{
	class MenuState :
		public State
	{
	public:
		MenuState( StateStack& stack, Context& cont ) :
			State( stack, cont )
		{}

		stateID_t GetID() const override
		{
			return STATE_MENU;
		}

		void OnPush() override
		{
			auto& exitButton = this->context.entityFactory->CreateEntity( this->context.entityManager->CreateEntity(), ENTITY_UI_EXIT_BUTTON, this->context );
			exitButton.AddGroup( GROUP_MENU_STATE );
			auto& exitButtonPos = exitButton.GetComponent<PositionComponent>();
			exitButtonPos.x -= exitButton.GetComponent<DrawableComponent>().sprite.getGlobalBounds().width / 2;
			this->context.entityFactory->CreateEntity( this->context.entityManager->CreateEntity(), ENTITY_UI_PLAY_BUTTON, this->context );
		}

		void OnPop() override
		{
			auto entitiesToKill = this->context.entityManager->GetEntitiesByGroup( GROUP_MENU_STATE );
			for ( auto entity : entitiesToKill )
				entity->Kill();
		}
	};
}