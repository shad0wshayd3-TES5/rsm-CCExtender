#include "EditorIDCache.h"

#include <cassert>
#include <typeinfo>

#include "SKSE/API.h"
#include "SKSE/CodeGenerator.h"
#include "SKSE/SafeWrite.h"


EditorIDCache* EditorIDCache::GetSingleton()
{
	static EditorIDCache singleton;
	return &singleton;
}


void EditorIDCache::InstallHooks()
{
	WritePatch(REL::ID(10883), unrestricted_cast<std::uintptr_t>(&Hook_SetFormEditorID));	// 1_5_97
	_MESSAGE("Installed hooks for (%s)", typeid(EditorIDCache).name());
}


std::string EditorIDCache::GetEditorID(RE::TESForm* a_form) const
{
	assert(a_form);

	auto id = a_form->GetFormEditorID();
	if (id && id[0] != '\0') {
		return id;
	}

	Locker locker(_lock);
	auto it = _idMap.find(a_form->GetFormID());
	return it != _idMap.end() ? it->second : "";
}


void EditorIDCache::WritePatch(REL::ID a_hookID, std::uintptr_t a_funcAddr)
{
	constexpr std::size_t CAVE_SIZE = 0x20;

	REL::Offset<std::uintptr_t> funcBase = a_hookID;

	struct Patch : SKSE::CodeGenerator
	{
		Patch(std::size_t a_callAddr) : SKSE::CodeGenerator(CAVE_SIZE)
		{
			Xbyak::Label callLbl;

			jmp(ptr[rip + callLbl]);

			L(callLbl);
			dq(a_callAddr);
		}
	};

	Patch patch(a_funcAddr);
	patch.finalize();

	for (std::size_t i = 0; i < patch.getSize(); ++i) {
		SKSE::SafeWrite8(funcBase.GetAddress() + i, patch.getCode()[i]);
	}
}


bool EditorIDCache::Hook_SetFormEditorID(RE::TESForm* a_this, const char* a_str)
{
	if (a_str) {
		auto singleton = GetSingleton();
		Locker locker(singleton->_lock);
		singleton->_idMap[a_this->GetFormID()] = a_str;
	}
	return true;
}
