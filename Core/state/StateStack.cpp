/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include <Core/state/StateStack.hpp>

namespace con
{
	void StateStack::ApplyPendingActions()
	{
		for ( auto& action : this->pendingActions )
			switch ( action.action )
			{
			case action_t::POP:
			{
				LOG( "State pop", INFO, CONSOLE );
				this->stack.back()->OnPop();
				this->stack.pop_back();
				break;
			}
			case action_t::PUSH:
			{
				if ( this->stack.empty() || this->stack.back()->GetID() != action.state )
				{
					LOG( "State push ( " << +action.state << " )", INFO, CONSOLE );
					this->stack.push_back( this->createState( action.state ) );
					this->stack.back()->OnPush();
				}
				break;
			}
			}

		this->pendingActions.clear();
	}

	void StateStack::Update()
	{
		if ( this->stack.empty() )
			return;

		for ( size_t i = 0; i < this->stack.size(); i++ )
			this->stack[i]->Update();
	}

	std::unique_ptr<State> StateStack::createState( stateID_t id )
	{
		auto result = this->factories.find( id );
		CON_ASSERT( !this->factories.empty() && result != this->factories.end(), "State of id " + std::to_string( +id ) + " isn't registered" );

		return result->second();
	}

	StateStack::pendingAction_t::pendingAction_t( action_t act, stateID_t stat ) :
		action( act ),
		state( stat )
	{}
}