#pragma once


constexpr const char* safe_cstr(const char* a_str)
{
	return a_str ? a_str : "";
}
