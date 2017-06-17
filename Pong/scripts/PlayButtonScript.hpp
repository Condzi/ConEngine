/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/scripts/ButtonScript.hpp>
#include <Core/state/Game.hpp>

#include <Pong/Enums.hpp>

namespace con
{
	struct PlayButtonScript :
		ButtonScript
	{
		void OnButtonClick() override
		{
			this->context->stateStack->Pop();
			this->context->stateStack->Push( STATE_PLAY );
		}
	};
}