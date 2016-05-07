#ifndef SETTINGS_CBASESETTINGS_H
#define SETTINGS_CBASESETTINGS_H

#include <memory>

#include "keyvalues/KVForward.h"

namespace filesystem
{
class IFileSystem;
}

namespace settings
{
class CGameConfig;
class CGameConfigManager;

/**
*	Listener for settings.
*	TODO: replace with cvars
*/
class ISettingsListener
{
public:
	virtual ~ISettingsListener() = 0;

	virtual void FPSChanged( const double flOldFPS, const double flNewFPS ) = 0;
};

inline ISettingsListener::~ISettingsListener()
{
}

/**
*	This class manages settings that are shared between all tools.
*	This class is abstract.
*	TODO: make dependency on filesystem use a member variable instead of a global
*/
class CBaseSettings
{
public:
	static const double DEFAULT_FPS;

	static const double MIN_FPS;

	static const double MAX_FPS;

protected:
	/**
	*	Constructs a default settings instance.
	*	@param pFileSystem Filesystem to use.
	*/
	CBaseSettings( filesystem::IFileSystem* const pFileSystem );

	/**
	*	Constructs settings using another settings object. This is a deep copy.
	*	@param other Settings to copy.
	*/
	CBaseSettings( const CBaseSettings& other );

	/**
	*	Sets the settings contained in this object to the given settings.
	*	@param other Settings to copy.
	*	@return *this
	*/
	CBaseSettings& operator=( const CBaseSettings& other );

public:
	/**
	*	Destructor.
	*/
	virtual ~CBaseSettings();

	/**
	*	Gets the settings listener.
	*/
	ISettingsListener* GetSettingsListener() { return m_pListener; }

	/**
	*	Sets the settings listener.
	*/
	void SetSettingsListener( ISettingsListener* const pListener )
	{
		m_pListener = pListener;
	}

	/**
	*	Gets the config manager.
	*/
	std::shared_ptr<const CGameConfigManager> GetConfigManager() const { return m_ConfigManager; }

	/**
	*	@see GetConfigManager() const
	*/
	std::shared_ptr<CGameConfigManager> GetConfigManager() { return m_ConfigManager; }

	/**
	*	Gets the FPS.
	*/
	double GetFPS() const { return m_flFPS; }

	/**
	*	Sets the FPS.
	*/
	void SetFPS( const double flFPS );

	/**
	*	Returns whether the settings object is initialized or not.
	*/
	bool IsInitialized() const { return m_bInitialized; }

	/**
	*	Called on startup. Initializes the settings object from a file.
	*	@param pszFilename Name of the file to load. If no file exists, the default settings are written to the file.
	*	@return true if the settings were successfully loaded, or saved, if no file by the given name existed. false otherwise.
	*/
	bool Initialize( const char* const pszFilename );

	/**
	*	Called on shutdown. Saves the settings object to a file.
	*	@param pszFilename Name of the file to save to.
	*/
	void Shutdown( const char* const pszFilename );

	/**
	*	Initializes the file system.
	*	@return true if the file system was successfully initialized, false otherwise.
	*/
	bool InitializeFileSystem();

	/**
	*	Loads settings from the given file.
	*	@param pszFilename File to load settings from.
	*	@return true on success, false otherwise.
	*/
	bool LoadFromFile( const char* const pszFilename );

	/**
	*	Saves settings to the given file.
	*	@param pszFilename File to save settings to.
	*	@return true on success, false otherwise.
	*/
	bool SaveToFile( const char* const pszFilename );

protected:
	/**
	*	Called after this object has been initialized.
	*	@see Initialize
	*/
	virtual bool PostInitialize( const char* const pszFilename ) { return true; }

	/**
	*	Called before this object is shut down.
	*	@see Shutdown
	*/
	virtual void PreShutdown( const char* const pszFilename ) {}

	/**
	*	Initializes the file system for the given configuration. Call the base class implementation first.
	*	@param config Configuration to initialize with.
	*	@see InitializeFileSystem()
	*/
	virtual bool InitializeFileSystem( const std::shared_ptr<const CGameConfig>& config );

	/**
	*	Loads settings from the given keyvalues block.
	*	@param root Root block.
	*	@see LoadFromFile( const char* const pszFilename )
	*/
	virtual bool LoadFromFile( const kv::Block& root );

	/**
	*	Saves settings using the given writer.
	*	@param writer Writer.
	*	@see SaveToFile( const char* const pszFilename )
	*/
	virtual bool SaveToFile( kv::Writer& writer );

	bool LoadCommonSettings( const kv::Block& root );

	bool SaveCommonSettings( kv::Writer& writer );

	bool LoadGameConfigs( const kv::Block& root );

	bool SaveGameConfigs( kv::Writer& writer );

private:
	//Note: must be non-virtual.
	void Copy( const CBaseSettings& other );

private:
	filesystem::IFileSystem* const m_pFileSystem;
	ISettingsListener* m_pListener = nullptr;

	/**
	*	The config manager. This keeps track of all game configurations, as well as the active configuration.
	*/
	std::shared_ptr<CGameConfigManager> m_ConfigManager = std::make_shared<CGameConfigManager>();

	double m_flFPS = DEFAULT_FPS;

	bool m_bInitialized = false;
};
}

#endif //SETTINGS_CBASESETTINGS_H