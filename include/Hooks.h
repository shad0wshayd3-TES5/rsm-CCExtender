#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "RE/Skyrim.h"


namespace Hooks
{
	namespace
	{
		class TESFormEx : public RE::TESForm
		{
		public:
			static void InstallHooks();

		private:
			using Lock = std::mutex;
			using Locker = std::lock_guard<Lock>;
			using EditorID = std::unique_ptr<std::string>;


			static void WritePatch(std::uintptr_t a_hookAddr, std::uintptr_t a_funcAddr);

			const char* Hook_GetEditorID();
			bool Hook_SetEditorID(const char* a_str);


			static Lock _lock;
			static std::unordered_map<RE::FormID, EditorID> _idMap;
		};
	}


	void Install();
}
