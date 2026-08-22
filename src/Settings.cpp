#include "Settings.h"

#include <glaze/glaze.hpp>

namespace REX::Impl
{
	template <>
	void JsonSettingLoad(void* a_data, std::string_view a_path, RE::NiColorA& a_value, RE::NiColorA& a_valueDefault)
	{
		const auto& json = *static_cast<glz::generic*>(a_data);
		if (a_path[0] != '/')
		{
			const auto path = std::format("/{}"sv, a_path);
			a_value = glz::get<RE::NiColorA>(json, path).value_or(a_valueDefault);
		}
		else
		{
			a_value = glz::get<RE::NiColorA>(json, a_path).value_or(a_valueDefault);
		}
	};

	template <>
	void JsonSettingSave(void* a_data, std::string_view a_path, RE::NiColorA& a_value)
	{
		auto& json = *static_cast<glz::generic*>(a_data);
		if (a_path[0] != '/')
		{
			const auto path = std::format("/{}"sv, a_path);
			glz::set(json, path, a_value);
		}
		else
		{
			glz::set(json, a_path, a_value);
		}
	};
}
