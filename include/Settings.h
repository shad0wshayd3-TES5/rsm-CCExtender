#pragma once

#include "Json2Settings.h"


namespace J2S = Json2Settings;


class Settings
{
public:
	Settings() = delete;

	static bool LoadSettings(bool a_dumpParse = false);


	static J2S::sSetting betaCommentFileName;

private:
	static inline constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\CCExtender.json";
};
