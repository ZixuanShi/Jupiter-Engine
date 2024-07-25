// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.SystemPaths;

export import jpt.File.Path;

export namespace jpt::File
{
	class SystemPaths
	{
	private:
		Path m_engineFolder;	/**< Engine folder root */
		Path m_clientFolder;	/**< Development project folder */
		Path m_outputFolder;	/**< Output folder */
		Path m_userFolder;		/**< User's documents folder */
		Path m_tempFolder;		/**< Temporary folder */

		Path m_exePath;			/**< Executable path */
		Path m_configPath;		/**< Configuration Path */
		Path m_logPath;			/**< Log file path */

	public:
		static SystemPaths& GetInstance();

		void Init();
	};

	SystemPaths& SystemPaths::GetInstance()
	{
		static SystemPaths instance;
		return instance;
	}

	void SystemPaths::Init()
	{
	}
}