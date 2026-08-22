#pragma once

namespace REX::JSON
{
	using ColorA = TJsonSetting<RE::NiColorA>;
}

namespace Settings
{
	static REX::JSON::Str    betaCommentFileName{ "betaCommentFileName"sv, "betacomments.txt"s };
	static REX::JSON::ColorA consoleSelectedRefColor{ "consoleSelectedRefColor"sv, { 0.51F, 0.61F, 0.62F, 0.5F } };

	inline void Load()
	{
		auto json = REX::JSON::SettingStore::GetSingleton();
		json->Init(
			"Data/SKSE/Plugins/CCExtender.json",
			"Data/SKSE/Plugins/CCExtenderCustom.json");
		json->Load();
	}
}
