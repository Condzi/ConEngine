/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include <Core/systems/SimplePhysic.hpp>

namespace con
{
	void SimplePhysicSystem::Update()
	{
		this->timeAccumulator += Time::FRAME_TIME;

		while( this->timeAccumulator > this->ups )
		{
			this->updatePhysic();
			this->timeAccumulator -= Time::FRAME_TIME;
		}
	}

	std::vector<SimplePhysicSystem::physicComponentsPack_t> SimplePhysicSystem::getSimulators()
	{
		auto entities = this->context.entityManager->GetEntitiesWithSignature( this->signature );
		entities.erase( std::remove_if( std::begin( entities ), std::end( entities ),
			[]( auto entity )
		{
			return !entity->IsActive() || !entity->IsAlive();
		} ), std::end( entities ) );

		std::vector<physicComponentsPack_t> sims;
		sims.reserve( entities.size() );

		for ( auto entity : entities )
			sims.push_back( this->createPhysicComponentPack( entity ) );

		return sims;
	}

	void SimplePhysicSystem::updatePositionByVelocity( std::vector<physicComponentsPack_t>& simulators )
	{
		for ( auto simulator : simulators )
		{
			if ( !simulator.velocity )
				continue;

			simulator.position->x += simulator.velocity->x * Time::FRAME_TIME.AsSeconds();
			simulator.position->y += simulator.velocity->y * Time::FRAME_TIME.AsSeconds();
		}
	}

	void SimplePhysicSystem::updatePhysic()
	{
		auto simulators = this->getSimulators();

		for ( auto fIt = simulators.begin(); fIt != simulators.end(); fIt++ )
		{
			physicComponentsPack_t packA = *fIt;
			BoundingBox<float> bbAupdated = packA.collider->boundingBox;
			if ( packA.velocity )
				bbAupdated.position = { packA.position->x + packA.velocity->x * Time::FRAME_TIME.AsSeconds(), packA.position->y + packA.velocity->y * Time::FRAME_TIME.AsSeconds() };

			for ( auto rIt = simulators.rbegin(); rIt != simulators.rend(); rIt++ )
			{
				if ( fIt->entity == rIt->entity )
					continue;
				physicComponentsPack_t packB = *rIt;

				collisionSide_t collisionSide = testBBcollision( bbAupdated, packB.collider->boundingBox );

				if ( collisionSide != COLLISION_SIDE_NONE )
				{
					if ( packA.collider->callback )
						packA.collider->callback( *packA.collider, *packB.collider, collisionSide );
					if ( packB.collider->isTrigger && packB.collider->callback )
						packB.collider->callback( *packB.collider, *packA.collider, invertCollisionSide( collisionSide ) );
				}
			}
		}
		this->updatePositionByVelocity( simulators );
	}

	SimplePhysicSystem::physicComponentsPack_t SimplePhysicSystem::createPhysicComponentPack( Entity * entity )
	{
		physicComponentsPack_t pack;
		pack.entity = entity;
		pack.position = &entity->GetComponent<PositionComponent>();
		pack.collider = &entity->GetComponent<SimpleColliderComponent>();
		if ( entity->HasComponent<VelocityComponent>() )
			pack.velocity = &entity->GetComponent<VelocityComponent>();

		return pack;
	}
}