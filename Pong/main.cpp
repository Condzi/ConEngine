/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include <SFML/System/Err.hpp>

#include <Core/state/Game.hpp>

#include <Pong/states/LoadingScreen.hpp>
#include <Pong/states/Pause.hpp>
#include <Pong/states/Play.hpp>

using namespace con;

/*
[-] - not solved; [o] - temporary solution; [+] solved

	BUGS LIST [b]
[+]b1. When ball is in trigger and it's position is reseting and paddle is colliding with borders (top / bottom) and 
	player tries to go into border paddle will stuck into border.
[+]b2. Sometimes when game starts at the beginning of the Play State there is a physic anomaly - ball is flying in 
	random direction for unknow reason.

	IDEAS LIST [i]
[+]i1. Redirect SFML errors to file output or something.
*/
int main()
{
	// Solution for i1: redirect SFML error stream to text file.
	std::ofstream sfmlOutput( "sfml_output.txt" );
	sf::err().rdbuf( sfmlOutput.rdbuf() );

#if !defined DEBUG
	bool exitProperly = true;
	{
		try
		#endif // DEBUG
		{
			Game game( "settings.ini" );
			game.RegisterState<LoadingScreenState>( STATE_LOADING_SCREEN );
			game.RegisterState<PauseState>( STATE_PAUSE );
			game.RegisterState<PlayState>( STATE_PLAY );

			game.Run( STATE_LOADING_SCREEN );
		}
	#if !defined DEBUG
		catch ( BaseException& baseException )
		{
			LOG( "Base Exception (assert) thrown.", ERROR, BOTH );
			showBasicExceptionData( baseException );
			exitProperly = false;
		}
		catch ( std::runtime_error& runtime )
		{
			LOG( "Runtime Exception thrown.", ERROR, BOTH );
			LOG( "Exception data: \"" << runtime.what() << "\"", ERROR, BOTH );
			exitProperly = false;
		}
		catch ( std::exception& ex )
		{
			LOG( "Exception thrown.", ERROR, BOTH );
			LOG( "Exception data: \"" << ex.what() << "\"", ERROR, BOTH );
			exitProperly = false;
		}

		if ( !exitProperly )
		{
			LOG( "Press enter to exit.", INFO, CONSOLE );
			std::cin.get();
		} else
		{
			LOG( "Game exited properly.", INFO, BOTH );
		}
	}

	return exitProperly ? 0 : 1;
#else
		return 0;
	#endif
}