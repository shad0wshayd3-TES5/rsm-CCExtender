#pragma once
#pragma warning(disable : 4702)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <Json2Settings.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>

#define DLLEXPORT __declspec(dllexport)
#define UNLEN 256

using namespace std::literals;

namespace logger = SKSE::log;

namespace stl
{
	using namespace SKSE::stl;

	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);

	[[nodiscard]] constexpr std::string_view safe_string(const char* a_str) noexcept
	{
		return a_str ? a_str : "";
	}
}

#ifdef SKYRIM_AE
# define OFFSET(se, ae) ae
#else
# define OFFSET(se, ae) se
#endif

#include "Settings.h"
#include "Version.h"
