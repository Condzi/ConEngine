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
		void Update() override
		{
			this->loadTextures();
			this->loadFonts();

			this->requestStackPop();
			if ( this->context.settings->GetBool( "DEBUG", "DEBUG_DATA" ) )
				this->requestStackPush( STATE_DEBUG_DATA );
			this->requestStackPush( STATE_PLAY );
		}

	private:
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
	};
}