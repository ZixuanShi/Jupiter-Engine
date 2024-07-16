export module Settings;

import jpt.TypeDefs;

export struct Settings
{
public:
	int32 Width = 1280;
	int32 Height = 720;
	bool Fullscreen = false;
	bool VSync = false;
	bool ShowFPS = true;
	bool ShowConsole = true;
	bool ShowMemory = true;
};