/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <functional>

#include <Core/ecs/Component.hpp>
#include <Core/BoundingBox.hpp>
#include <Core/components/Velocity.hpp>

namespace con
{
	/*
	===============================================================================
	Created by: Condzi
		Component for collision detection. You can lock its X and Y axis, so it 
		won't move.

	===============================================================================
	*/
	struct SimpleColliderComponent final :
		Component
	{
		BoundingBox<float> boundingBox;
		bool isTrigger = false;
		bool lockXaxis = false, lockYaxis = false;

		void CorrectAfterCollision( const BoundingBox<float>& second, collisionSide_t side )
		{
			if ( !this->entity->HasComponent<PositionComponent>() )
				return;

			if ( this->lockXaxis && this->lockYaxis )
				return;

			auto& position = this->entity->GetComponent<PositionComponent>();
			if ( !this->lockXaxis )
			{
				if ( side == COLLISION_SIDE_LEFT )
					position.x = second.position.x - this->boundingBox.size.x;
				else if ( side == COLLISION_SIDE_RIGHT )
					position.x = second.position.x + second.size.x;
			}
			if ( !this->lockYaxis )
			{
				if ( side == COLLISION_SIDE_TOP )
					position.y = second.position.y + second.size.y;
				else if ( side == COLLISION_SIDE_BOTTOM )
					position.y = second.position.y - this->boundingBox.size.y;
			}
		}

		void StopInCollisionAxis( collisionSide_t side )
		{
			if ( !this->entity->HasComponent<VelocityComponent>() )
				return;
			auto& velocity = this->entity->GetComponent<VelocityComponent>();

			if ( side == COLLISION_SIDE_LEFT || side == COLLISION_SIDE_RIGHT )
				velocity.x = 0.0f;
			else
				velocity.y = 0.0f;
		}
	};
}