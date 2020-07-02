#pragma once

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

	static inline J2S::sSetting betaCommentFileName{ "betaCommentFileName", "betacomments.txt" };
	static inline J2S::oSetting<RE::NiColorA> consoleSelectedRefColor{ "consoleSelectedRefColor", std::in_place_t(), 0.51F, 0.61F, 0.62F, 0.5F };

private:
	static inline constexpr char FILE_NAME[] = "Data\\SKSE\\Plugins\\CCExtender.json";
};
