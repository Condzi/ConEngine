/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <cinttypes>

namespace con
{
	/*
	===============================================================================
	Created by: Condzi
		Wrapper class helping ResourceHolder in managing.
		Usage: ResourceWrapper<TYPE> resource.

	===============================================================================
	*/
	template <typename Resource>
	class ResourceWrapper final :
		public Resource
	{
	public:
		ResourceWrapper() :
			priority( 0 ), resourceID( 0 )
		{}
		ResourceWrapper( const Resource& res ) :
			Resource( res ),
			priority( 0 ), resourceID( 0 )
		{}
		ResourceWrapper( uint8_t prior, uint8_t id ) :
			priority( prior ),
			resourceID( id )
		{}

		uint8_t GetResourcePriority()
		{
			return this->priority;
		}
		uint8_t GetResourceID()
		{
			return this->resourceID;
		}

		void SetResourcePriority( uint8_t val )
		{
			this->priority = val;
		}
		void SetResourceID( uint8_t val )
		{
			this->resourceID = val;
		}

	private:
		uint8_t priority, resourceID;
	};
}