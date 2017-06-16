/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace con
{
	/*
	===============================================================================
	Created by: Condzi
		Class for logging purposes. Use static method Log(). Use macro MSG to create
		a message (MSG<< "My data" << " 123") or use macro LOG and DEBUG_LOG.

	===============================================================================
	*/
	class Logger final
	{
	public:
		enum prefix_t : uint8_t // IDs are also console text colors.
		{
			INFO = 0xF,
			WARNING = 0xE,
			ERROR = 0xC
		};
		enum output_t : uint8_t
		{
			CONSOLE,
			FILE,
			BOTH
		};

		static void Log( std::ostream& message, prefix_t prefix = INFO, output_t output = CONSOLE );

	private:
		std::ofstream outputFile;

		Logger();

		void logToConsole( const std::string& message );
		void logToFile( const std::string& message );

		static void setTextColor( uint8_t col );
	};
}

// For IntelliSense autocomplete.

#define INFO INFO
#define WARNING WARNING
#define ERROR ERROR
#define CONSOLE CONSOLE
#define FILE FILE
#define BOTH BOTH

#define MSG std::ostringstream().flush()

#define LOG( msg, prefix, output ) Logger::Log( MSG << msg, Logger::prefix, Logger::output )
#if defined DEBUG
#define DEBUG_LOG( msg, prefix, output ) LOG( msg, prefix, output )
#else
#if defined _MSC_VER
#define DEBUG_LOG( msg, prefix, output ) __noop
#else
#define DEBUG_LOG( msg, prefix, output )
#endif // __MSC_VER
#endif // DEBUG