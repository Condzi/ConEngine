/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/System/Vector2.hpp>

namespace con
{
	enum collisionSide_t : uint8_t
	{
		COLLISION_SIDE_NONE = 0,

		COLLISION_SIDE_LEFT,
		COLLISION_SIDE_RIGHT,
		COLLISION_SIDE_TOP,
		COLLISION_SIDE_BOTTOM,
	};

	inline collisionSide_t invertCollisionSide( const collisionSide_t src )
	{
		switch ( src )
		{
		case COLLISION_SIDE_LEFT: return COLLISION_SIDE_RIGHT;
		case COLLISION_SIDE_RIGHT: return COLLISION_SIDE_LEFT;
		case COLLISION_SIDE_TOP: return COLLISION_SIDE_BOTTOM;
		case COLLISION_SIDE_BOTTOM: return COLLISION_SIDE_TOP;
		case COLLISION_SIDE_NONE: return COLLISION_SIDE_NONE;
		}
		return COLLISION_SIDE_NONE;
	}

	/*
	===============================================================================
	Created by: Condzi
		2D rectangle, used for collision detection. Similar to sf::Rect<T>.

	===============================================================================
	*/
	template <typename T>
	struct BoundingBox
	{
		sf::Vector2<T> position, size;

		BoundingBox( const sf::Vector2<T>& pos = sf::Vector2<T>(), const sf::Vector2<T>& sz = sf::Vector2<T>() ) :
			position( pos ), size( sz )
		{}
		BoundingBox( const T& x, const T& y, const T& width, const T& height ) :
			position( x, y ), size( width, height )
		{}

		bool Intersects( const BoundingBox<T>& second, BoundingBox<T>* intersection = nullptr ) const
		{
			// Rectangles with negative dimensions are allowed, so we must handle them correctly
			// Compute the min and max of the first rectangle on both axes
			T r1MinX = std::min( this->position.x, static_cast<T>( this->position.x + this->size.x ) );
			T r1MaxX = std::max( this->position.x, static_cast<T>( this->position.x + this->size.x ) );
			T r1MinY = std::min( this->position.y, static_cast<T>( this->position.y + this->size.y ) );
			T r1MaxY = std::max( this->position.y, static_cast<T>( this->position.y + this->size.y ) );
			// Compute the min and max of the second rectangle on both axes
			T r2MinX = std::min( second.position.x, static_cast<T>( second.position.x + second.size.x ) );
			T r2MaxX = std::max( second.position.x, static_cast<T>( second.position.x + second.size.x ) );
			T r2MinY = std::min( second.position.y, static_cast<T>( second.position.y + second.size.y ) );
			T r2MaxY = std::max( second.position.y, static_cast<T>( second.position.y + second.size.y ) );
			// Compute the intersection boundaries
			T interLeft = std::max( r1MinX, r2MinX );
			T interTop = std::max( r1MinY, r2MinY );
			T interRight = std::min( r1MaxX, r2MaxX );
			T interBottom = std::min( r1MaxY, r2MaxY );

			if ( intersection )
				*intersection = BoundingBox<T>( interLeft, interTop, interRight - interLeft, interBottom - interTop );
			// If the intersection is valid (positive non zero area), then there is an intersection
			return ( interLeft < interRight ) && ( interTop < interBottom );
		}
	};

	template <typename T>
	inline collisionSide_t testBBcollision( const BoundingBox<T>& first, const BoundingBox<T>& second )
	{
		BoundingBox<T> intersection;
		if ( !first.Intersects( second, &intersection ) )
			return COLLISION_SIDE_NONE;

		if ( intersection.size.x > intersection.size.y )
		{
			if ( first.position.y > second.position.y )
				return COLLISION_SIDE_TOP;
			else
				return COLLISION_SIDE_BOTTOM;
		} else
		{
			if ( first.position.x < second.position.x )
				return COLLISION_SIDE_LEFT;
			else
				return COLLISION_SIDE_RIGHT;
		}
	}
}