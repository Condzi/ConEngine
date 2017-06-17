/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include <Core/Settings.hpp>

// IMPORTANT: Remove redundancy.

namespace con
{
	settingsCategoryInterface_t* SettingsContainer::GetSettings( settings_t id ) const
	{
		return this->settings[id].get();
	}
	void SettingsContainer::LoadSettings( settings_t id, bool* error )
	{
		this->settings[id] = std::make_unique<settingsCategory_t>( this );
		this->settings[id]->Load( this->getSettingsFilename( id ), error );
	}

	void SettingsContainer::SaveSettings( settings_t id )
	{
		this->settings[id]->Save( this->getSettingsFilename( id ) );
	}

	std::string SettingsContainer::getSettingsFilename( settings_t id )
	{
		CON_ASSERT( internal::getSettingsFilename( id ), "invalid settings id" );
		return internal::getSettingsFilename( id );
	}

	bool* settingsCategory_t::GetBoolean( std::string section, std::string name, bool* error )
	{
		bool internalError = false;
		auto sectionID = this->findSection( section, this->booleans, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find settings section '" << section << "'", ERROR, BOTH );
			return nullptr;
		}
		auto& sectionVec = this->booleans[sectionID].second;
		auto valueID = this->findValue( name, sectionVec, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find boolean in section '" << section << "' of name '" << name << "'", ERROR, BOTH );
			return nullptr;
		}

		return &sectionVec[valueID].second;
	}

	int* settingsCategory_t::GetInt( std::string section, std::string name, bool* error )
	{
		bool internalError = false;
		auto sectionID = this->findSection( section, this->ints, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find settings section '" << section << "'", ERROR, BOTH );
			return nullptr;
		}
		auto& sectionVec = this->ints[sectionID].second;
		auto valueID = this->findValue( name, sectionVec, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find int in section '" << section << "' of name '" << name << "'", ERROR, BOTH );
			return nullptr;
		}

		return &sectionVec[valueID].second;
	}

	double* settingsCategory_t::GetDouble( std::string section, std::string name, bool* error )
	{
		bool internalError = false;
		auto sectionID = this->findSection( section, this->doubles, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find settings section '" << section << "'", ERROR, BOTH );
			return nullptr;
		}
		auto& sectionVec = this->doubles[sectionID].second;
		auto valueID = this->findValue( name, sectionVec, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find double in section '" << section << "' of name '" << name << "'", ERROR, BOTH );
			return nullptr;
		}

		return &sectionVec[valueID].second;
	}

	std::string* settingsCategory_t::GetString( std::string section, std::string name, bool * error )
	{
		bool internalError = false;
		auto sectionID = this->findSection( section, this->strings, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find settings section '" << section << "'", ERROR, BOTH );
			return nullptr;
		}
		auto& sectionVec = this->strings[sectionID].second;
		auto valueID = this->findValue( name, sectionVec, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot find string in section '" << section << "' of name '" << name << "'", ERROR, BOTH );
			return nullptr;
		}

		return &sectionVec[valueID].second;
	}

	void settingsCategory_t::AddBoolean( std::string section, std::string name, bool value )
	{
		auto valPtr = this->GetBoolean( section, name );
		if ( valPtr )
			*valPtr = value;

		std::vector<std::pair<std::string, bool>> sectionData;
		sectionData.emplace_back( std::make_pair( name, value ) );

		bool internalError = false;
		auto sectionID = this->findSection( section, this->booleans, &internalError );
		if ( internalError )
			this->booleans.emplace_back( std::make_pair( section, sectionData ) );
		else
			this->booleans[sectionID].second.push_back( std::make_pair( name, value ) );
	}

	void settingsCategory_t::AddInt( std::string section, std::string name, int value )
	{
		auto valPtr = this->GetInt( section, name );
		if ( valPtr )
			*valPtr = value;

		std::vector<std::pair<std::string, int>> sectionData;
		sectionData.emplace_back( std::make_pair( name, value ) );

		bool internalError = false;
		auto sectionID = this->findSection( section, this->ints, &internalError );
		if ( internalError )
			this->ints.emplace_back( std::make_pair( section, sectionData ) );
		else
			this->ints[sectionID].second.push_back( std::make_pair( name, value ) );
	}

	void settingsCategory_t::AddDouble( std::string section, std::string name, double value )
	{
		auto valPtr = this->GetDouble( section, name );
		if ( valPtr )
			*valPtr = value;

		std::vector<std::pair<std::string, double>> sectionData;
		sectionData.emplace_back( std::make_pair( name, value ) );

		bool internalError = false;
		auto sectionID = this->findSection( section, this->doubles, &internalError );
		if ( internalError )
			this->doubles.emplace_back( std::make_pair( section, sectionData ) );
		else
			this->doubles[sectionID].second.push_back( std::make_pair( name, value ) );
	}

	void settingsCategory_t::AddString( std::string section, std::string name, std::string value )
	{
		auto valPtr = this->GetString( section, name );
		if ( valPtr )
			*valPtr = value;

		std::vector<std::pair<std::string, std::string>> sectionData;
		sectionData.emplace_back( std::make_pair( name, value ) );

		bool internalError = false;
		auto sectionID = this->findSection( section, this->strings, &internalError );
		if ( internalError )
			this->strings.emplace_back( std::make_pair( section, sectionData ) );
		else
			this->strings[sectionID].second.push_back( std::make_pair( name, value ) );
	}

	void settingsCategory_t::Load( std::string path, bool* error )
	{
		bool internalError = false;
		this->loadFromFile( path, &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Cannot load settings from path '" << path << "'", ERROR, BOTH );
			return;
		}
		this->parse( &internalError );
		if ( internalError )
		{
			if ( error )
				*error = true;
			LOG( "Settings file ('" << path << "') failed to parse", ERROR, BOTH );
		}
	}

	void settingsCategory_t::Save( std::string path )
	{
		this->serialize();
		this->saveToFile( path );
	}
}