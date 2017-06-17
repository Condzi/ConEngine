/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Core/scripts/ButtonScript.hpp>
#include <Core/state/Game.hpp>

namespace con
{
	struct ExitButtonScript :
		ButtonScript
	{
		void OnButtonClick() override
		{
			this->context->game->Exit();
		}
	};
}