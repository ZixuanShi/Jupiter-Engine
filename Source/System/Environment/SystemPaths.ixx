// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#if IS_PLATFORM_WIN64
	#include <Windows.h>
#endif

export module jpt.System.Paths;

import jpt.File.Path;

using namespace jpt::File;

namespace jpt::System
{
	export class Paths
	{
	private:
		Path m_engineDir;
		Path m_clientDir;
		Path m_outputDir;
		Path m_savedDir;

		Path m_executablePath;

		bool m_isInitialized = false;

	public:
		static Paths& GetInstance();
		void PreInit(const Path& clientDir, const Path& outputDir);

		const Path& GetEngineDir()      const { JPT_ASSERT(m_isInitialized); return m_engineDir;      }
		const Path& GetClientDir()      const { JPT_ASSERT(m_isInitialized); return m_clientDir;      }
		const Path& GetOutputDir()      const { JPT_ASSERT(m_isInitialized); return m_outputDir;      }
		const Path& GetSavedDir()       const { JPT_ASSERT(m_isInitialized); return m_savedDir;       }
		const Path& GetExecutablePath() const { JPT_ASSERT(m_isInitialized); return m_executablePath; }
	};

	Paths& Paths::GetInstance()
	{
		static Paths instance;
		return instance;
	}

	void Paths::PreInit(const Path& clientDir, const Path& outputDir)
	{
		m_engineDir = JPT_ENGINE_DIR_W;
		m_clientDir = clientDir;
		m_outputDir = outputDir;
		m_savedDir = clientDir + L"_Saved";

#if IS_PLATFORM_WIN64
		wchar_t buffer[MAX_PATH];
		GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		m_executablePath = buffer;
#endif

		JPT_ASSERT(!m_engineDir.IsEmpty());
		JPT_ASSERT(!m_clientDir.IsEmpty());
		JPT_ASSERT(!m_outputDir.IsEmpty());
		m_isInitialized = true;
	}
}