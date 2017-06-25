/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/state/State.hpp>
#include <Core/ecs/Entity.hpp>
#include <Core/components/Drawable.hpp>
#include <Core/components/Position.hpp>

#include <Pong/Enums.hpp>
#include <Pong/EntityCreators.hpp>

namespace con
{
	class LoadingScreenState :
		public State
	{
	public:
		LoadingScreenState( StateStack& stack, Context& cont ) :
			State( stack, cont )
		{}

		stateID_t GetID() const override { return STATE_LOADING_SCREEN; }

		void OnPush() override
		{
			this->StartThread();
		}
		void OnPop() override
		{
			this->StopThread();
		}

		// Loads and configures everything.
		void UpdateThread() override
		{
			// Thread tries to call UpdateThread multiple times; until StateStack call OnPop and stop this madness.
			if ( this->resourcesLoaded )
				return;
			this->resourcesLoaded = true;
			this->loadTextures();
			this->loadFonts();
			this->registerEntityCreators();

			this->requestStackPop();
			this->requestStackPush( STATE_MENU );
		}

	private:
		bool resourcesLoaded = false;
		void loadTextures()
		{
			auto& cache = this->context.resourceCache;

			cache->textures.emplace_back( std::make_unique<textureResource_t>( RESOURCE_MULTISTATE, TEXTURE_SHEET ) );
			auto& sheet = cache->textures.back();
			if ( !sheet->loadFromFile( "TextureSheet.png" ) )
			{
				cache->textures.pop_back();
				CON_ASSERT( false, "Cannot load TextureSheet.png" );
			}

			cache->textures.emplace_back( std::make_unique<textureResource_t>( RESOURCE_MULTISTATE, TEXTURE_BACKGROUND ) );
			auto& background = cache->textures.back();
			if ( !background->loadFromFile("Background.png") )
			{
				cache->textures.pop_back();
				LOG( "Cannot load Background.png", ERROR, BOTH );
			}
		}

		void loadFonts()
		{
			auto& cache = this->context.resourceCache;

			cache->fonts.emplace_back( std::make_unique<fontResource_t>( RESOURCE_MULTISTATE, FONT_DEFAULT ) );
			auto& font = cache->fonts.back();
			if ( !font->loadFromFile( "GiantRobotArmy-Medium.ttf" ) )
			{
				cache->fonts.pop_back();
				CON_ASSERT( false, "Cannot load GiantRobotArmy-Medium.ttf" );
			}
		}

		void registerEntityCreators()
		{
			auto entityFactory = this->context.entityFactory;
			entityFactory->AddCreator<PaddleACreator>();
			entityFactory->AddCreator<PaddleBCreator>();
			entityFactory->AddCreator<PaddleArkanoid>();
			entityFactory->AddCreator<BorderCreator>();
			entityFactory->AddCreator<TriggerCreator>();
			entityFactory->AddCreator<BallCreator>();
			entityFactory->AddCreator<UIPointsCreator>();
			entityFactory->AddCreator<UIPauseCreator>();
			entityFactory->AddCreator<UIExitButtonCreator>();
			entityFactory->AddCreator<UIPlayButtonCreator>();
			entityFactory->AddCreator<BackgroundCreator>();
		}
	};
}