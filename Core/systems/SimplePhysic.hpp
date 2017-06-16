/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/ecs/EntityManager.hpp>
#include <Core/ecs/System.hpp>
#include <Core/ecs/Functions.hpp>
#include <Core/components/Position.hpp>
#include <Core/components/SimpleCollider.hpp>
#include <Core/components/Velocity.hpp>
// FRAME_TIME
#include <Core/time/Time.hpp>
// Context structure.
#include <Core/state/State.hpp>
#include <Core/Settings.hpp>

namespace con
{
	/*
	===============================================================================
	Created by: Condzi
		Updates velocity of Entities, detects collision.

	===============================================================================
	*/
	class SimplePhysicSystem final :
		public System
	{
		struct physicComponentsPack_t
		{
			Entity* entity = nullptr;
			PositionComponent* position = nullptr;
			SimpleColliderComponent* collider = nullptr;
			VelocityComponent* velocity = nullptr;
		};

	public:
		SimplePhysicSystem( Context& cont ) :
			System( cont )
		{}

		systemID_t GetID() const override
		{
			return 1;
		}

		void Init() override
		{
			this->signature = createComponentSignature( getComponentTypeID<PositionComponent>(), getComponentTypeID<SimpleColliderComponent>() );
			this->ups = asSeconds( 1.0f / static_cast<uint8_t>( this->context.settings->GetInt( "PHYSIC", "UPS" ) ) );
		}

		void Update() override;

	private:
		componentBitset_t signature;
		Time timeAccumulator;
		Time ups;

		void updatePhysic();
		physicComponentsPack_t createPhysicComponentPack( Entity* entity );
		std::vector<physicComponentsPack_t> getSimulators();
		void updatePositionByVelocity( std::vector<physicComponentsPack_t>& simulators );
	};
}