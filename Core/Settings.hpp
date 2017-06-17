#pragma once

#include <Core/Config.hpp>
#include <Core/fileManaging/INIparser.hpp>
#include <Core/FixedArray.hpp>
#include <Core/Assert.hpp>

namespace con
{
	enum settings_t : uint8_t
	{
		SETTINGS_ENGINE_DEFAULT = 0,
		SETTINGS_ENGINE,
		SETTINGS_GAME_DEFAULT,
		SETTINGS_GAME
	};

	namespace internal
	{
		constexpr const char* getSettingsFilename( settings_t id )
		{
			switch ( id )
			{
			case SETTINGS_ENGINE_DEFAULT:
			case SETTINGS_ENGINE: return "engine.ini";
			case SETTINGS_GAME_DEFAULT:
			case SETTINGS_GAME: return "game.ini";
			}
			return nullptr;
		}
	}

	class SettingsContainer;

	struct settingsCategoryInterface_t
	{
		virtual ~settingsCategoryInterface_t() {}
		virtual bool* GetBoolean( std::string section, std::string name, bool* error = nullptr ) = 0;
		virtual int* GetInt( std::string section, std::string name, bool* error = nullptr ) = 0;
		virtual double* GetDouble( std::string section, std::string name, bool* error = nullptr ) = 0;
		virtual std::string* GetString( std::string section, std::string name, bool* error = nullptr ) = 0;

		virtual void AddBoolean( std::string section, std::string name, bool value ) = 0;
		virtual void AddInt( std::string section, std::string name, int value ) = 0;
		virtual void AddDouble( std::string section, std::string name, double value ) = 0;
		virtual void AddString( std::string section, std::string name, std::string value ) = 0;

		virtual void Load( std::string path, bool* error ) = 0;
		virtual void Save( std::string path ) = 0;
		virtual bool Compare( settingsCategoryInterface_t* second ) = 0;
	};

	struct settingsCategory_t final :
		settingsCategoryInterface_t
	{
		SettingsContainer& container;

		std::vector<std::pair<std::string, std::vector<std::pair<std::string, bool>>>> booleans;
		std::vector<std::pair<std::string, std::vector<std::pair<std::string, int>>>> ints;
		std::vector<std::pair<std::string, std::vector<std::pair<std::string, double>>>> doubles;
		std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> strings;

		settingsCategory_t( SettingsContainer* settingsContainer ) :
			container( *settingsContainer )
		{}

		bool* GetBoolean( std::string section, std::string name, bool* error = nullptr ) override;
		int* GetInt( std::string section, std::string name, bool* error = nullptr ) override;
		double* GetDouble( std::string section, std::string name, bool* error = nullptr ) override;
		std::string* GetString( std::string section, std::string name, bool* error = nullptr ) override;

		void AddBoolean( std::string section, std::string name, bool value ) override;
		void AddInt( std::string section, std::string name, int value ) override;
		void AddDouble( std::string section, std::string name, double value ) override;
		void AddString( std::string section, std::string name, std::string value ) override;

		void Load( std::string path, bool* error = nullptr ) override;
		void Save( std::string path ) override;
		bool Compare( settingsCategoryInterface_t* second ) override;

	private:
		template< typename T>
		size_t findSection( std::string name, std::vector<std::pair<std::string, T>>& vec, bool* error = nullptr )
		{
			for ( size_t i = 0; i < vec.size(); i++ )
				if ( vec[i].first == name )
					return i;

			if ( error )
				*error = true;
			return 0;
		}
		template< typename T>
		size_t findValue( std::string name, std::vector<std::pair<std::string, T>>& vec, bool* error = nullptr )
		{
			// Algorighm is basicly the same.
			return this->findSection( name, vec, error );
		}

		std::vector<std::string> loadFromFile( std::string& path, bool* error  );
		void parse( bool* error );
		std::vector<std::string> serialize();
		void saveToFile( std::string& path );
	};

	class SettingsContainer
	{
	public:
		settingsCategoryInterface_t* GetSettings( settings_t id ) const;

		void LoadSettings( settings_t id, bool* error = nullptr );
		void SaveSettings( settings_t id );

	private:
		FixedArray<std::unique_ptr<settingsCategoryInterface_t>, 4> settings;
		std::string getSettingsFilename( settings_t id );
	};
	/*
	===============================================================================
	Created by: Condzi
		Simple extension for INIFile class. To get settings simple use Get methods from
		INIFile.

	===============================================================================
	*/
	class Settings_Old final :
		public INIFile
	{
	public:
		void GenerateDefault( const std::string& path )
		{
			// IDEA: constexpr 3D demensional table with this values?
			this->AddString( "WINDOW", "TITLE", GAME_NAME );
			this->AddInt( "WINDOW", "FPS", 64 );
			this->AddInt( "WINDOW", "X", 1280 );
			this->AddInt( "WINDOW", "Y", 720 );
			this->AddInt( "WINDOW", "DESIGNED_X", 1920 );
			this->AddInt( "WINDOW", "DESIGNED_Y", 1080 );
			this->AddInt( "PHYSIC", "UPS", 30 );

			this->SaveToFile( path, "; Default config. Don't change if you don't know what are you doing!" );
		}
	};
}