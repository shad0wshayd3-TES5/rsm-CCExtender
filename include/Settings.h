#pragma once

#include "Json2Settings.h"


class Settings
{
public:
	using sSetting = Json2Settings::sSetting;


	Settings() = delete;

	static bool LoadSettings(bool a_dumpParse = false);


	static sSetting betaCommentFileName;

private:
	static inline constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\CCExtender.json";
};
