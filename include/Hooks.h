#pragma once

#include <memory>
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
			static void WritePatch(std::uintptr_t a_hookAddr, std::uintptr_t a_funcAddr);
			static void InstallHooks();

			const char* Hook_GetEditorID();
			bool Hook_SetEditorID(const char* a_str);


			using EditorID = std::unique_ptr<std::string>;
			static std::unordered_map<RE::FormID, EditorID> _idMap;
		};
	}


	void Install();
}
