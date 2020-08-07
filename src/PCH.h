#pragma once

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/SKSE.h"

#include "Json2Settings.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>

#include <lmcons.h>

#include <boost/algorithm/searching/knuth_morris_pratt.hpp>
#include <robin_hood.h>

#ifndef NDEBUG
#include <spdlog/sinks/msvc_sink.h>
#else
#include <spdlog/sinks/basic_file_sink.h>
#endif

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using SKSE::util::to_underlying;
using SKSE::util::unrestricted_cast;

namespace logger = SKSE::log;

namespace stl
{
	using SKSE::stl::enumeration;
	using SKSE::stl::span;
}

class StopWatch
{
public:
	inline void Start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	inline void TimeStamp()
	{
		const auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - _start;
		logger::info(FMT_STRING("Milestone: {}s"), diff.count());
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

[[nodiscard]] constexpr std::string_view safe_string(const char* a_str) noexcept
{
	return a_str ? a_str : "";
}

#include "Settings.h"
