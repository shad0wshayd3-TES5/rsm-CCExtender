#pragma once

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/SKSE.h"

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

#include "Json2Settings.h"

#define DLLEXPORT __declspec(dllexport)

namespace stl
{
	using RE::stl::span;
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
		_MESSAGE("Milestone: %gs", diff.count());
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

[[nodiscard]] constexpr std::string_view safe_string(const char* a_str) noexcept
{
	return a_str ? a_str : "";
}

// https://en.wikipedia.org/wiki/Knuth-Morris-Pratt_algorithm
inline void kmp_table(const stl::span<const char>& W, std::vector<std::size_t>& T)
{
	constexpr auto NPOS = static_cast<std::size_t>(-1);

	std::size_t pos = 1;
	std::size_t cnd = 0;

	T[0] = NPOS;

	while (pos < W.size()) {
		if (W[pos] == W[cnd]) {
			T[pos] = T[cnd];
		} else {
			T[pos] = cnd;
			cnd = T[cnd];
			while (cnd != NPOS && W[pos] != W[cnd]) {
				cnd = T[cnd];
			}
		}
		++pos;
		++cnd;
	}

	T[pos] = cnd;
}

inline bool kmp_search(const stl::span<const char>& S, const stl::span<const char>& W)
{
	constexpr auto NPOS = static_cast<std::size_t>(-1);

	std::size_t j = 0;
	std::size_t k = 0;
	std::vector<std::size_t> T(W.size() + 1);
	kmp_table(W, T);

	while (j < S.size()) {
		if (W[k] == S[j]) {
			++j;
			++k;
			if (k == W.size()) {
				return true;
			}
		} else {
			k = T[k];
			if (k == NPOS) {
				++j;
				++k;
			}
		}
	}

	return false;
}
