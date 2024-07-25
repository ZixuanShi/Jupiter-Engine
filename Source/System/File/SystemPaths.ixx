// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.SystemPaths;

export import jpt.File.Path;

export namespace jpt::File
{
	class SystemPaths
	{
	private:
		Path m_engine;
		Path m_client;	/**< release config: Output build folder. other: Development project folder */
		Path m_user;	/**< User's documents folder */
		Path m_temp;	/**< Temporary folder */
		Path m_exe;		/**< Executable folder */

	public:
		static SystemPaths& GetInstance();

		void Init(const Path& engine, const Path& client);
	};

	SystemPaths& SystemPaths::GetInstance()
	{
		static SystemPaths instance;
		return instance;
	}

	void SystemPaths::Init(const Path& engine, const Path& client)
	{
		m_engine = engine;
		m_client = client;
	}
}