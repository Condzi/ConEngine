/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/ecs/Component.hpp>
#include <Core/Config.hpp>

namespace con
{
	// Forward declarations.
	struct Context;
	struct SimpleColliderComponent;
	enum collisionSide_t;

	struct ScriptComponent :
		Component
	{
		Context* context = nullptr;

		virtual void OnKill() {}

		virtual void OnSleep() {}
		virtual void OnActive() {}
		// Called every frame even if parent entity is not active.
		virtual void FixedUpdate() {}
		// Called every frame.
		virtual void Update() {}

		virtual void OnCollision( SimpleColliderComponent& first, SimpleColliderComponent& second, collisionSide_t side )
		{
			CON_UNUSED_PARAM( first );
			CON_UNUSED_PARAM( second );
			CON_UNUSED_PARAM( side );
		}
	};
}
