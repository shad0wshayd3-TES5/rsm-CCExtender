#pragma once

namespace RE
{
	inline void from_json(const nlohmann::json& a_json, RE::NiColorA& a_color)
	{
		using value_t = nlohmann::json::value_t;

		const std::array names{
			"red"s,
			"green"s,
			"blue"s,
			"alpha"s
		};

		for (std::size_t i = 0; i < names.size(); ++i)
		{
			a_color[i] = a_json.at(names[i]).get<float>();
			a_color[i] = std::clamp(a_color[i], 0.0F, 1.0F);
		}
	}
}

class Settings
{
public:
	Settings() = delete;

	inline static bool LoadSettings(bool a_dumpParse = false)
	{
		auto [log, success] = Json2Settings::load_settings(FILE_NAME, a_dumpParse);
		if (!log.empty())
		{
			logger::error{ fmt::runtime(log) };
		}
		return success;
	}

	inline static Json2Settings::sSetting betaCommentFileName{ "betaCommentFileName", "betacomments.txt" };
	inline static Json2Settings::oSetting<RE::NiColorA> consoleSelectedRefColor{ "consoleSelectedRefColor", std::in_place_t(), 0.51F, 0.61F, 0.62F, 0.5F };

private:
	inline static constexpr char FILE_NAME[] = "Data/SKSE/Plugins/CCExtender.json";
};
