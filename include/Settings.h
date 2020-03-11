#pragma once

#include <algorithm>
#include <array>

#include "RE/Skyrim.h"

#include "Json2Settings.h"


namespace J2S = Json2Settings;


namespace RE
{
	inline void from_json(const nlohmann::json& a_json, RE::NiColorA& a_color)
	{
		using value_t = nlohmann::json::value_t;

		std::array<std::string, 4> NAMES = {
			"red",
			"green",
			"blue",
			"alpha"
		};

		for (std::size_t i = 0; i < NAMES.size(); ++i) {
			a_color[i] = a_json.at(NAMES[i]).get<float>();
			a_color[i] = std::clamp<float>(a_color[i], 0.0, 1.0);
		}
	}
}


class Settings
{
public:
	Settings() = delete;

	static bool LoadSettings(bool a_dumpParse = false);


	static J2S::sSetting betaCommentFileName;
	static J2S::oSetting<RE::NiColorA> consoleSelectedRefColor;

private:
	static inline constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\CCExtender.json";
};
