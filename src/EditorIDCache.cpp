#include "EditorIDCache.h"

#include <cassert>
#include <typeinfo>

#include "skse64_common/SafeWrite.h"
#include "xbyak/xbyak.h"

#include "SKSE/API.h"
#include "SKSE/Trampoline.h"


EditorIDCache* EditorIDCache::GetSingleton()
{
	static EditorIDCache singleton;
	return &singleton;
}


void EditorIDCache::InstallHooks()
{
	WritePatch(0x000F72E0, unrestricted_cast<std::uintptr_t>(&Hook_SetFormEditorID));	// 1_5_97
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


void EditorIDCache::WritePatch(std::uintptr_t a_hookAddr, std::uintptr_t a_funcAddr)
{
	constexpr std::size_t CAVE_SIZE = 0x20;

	REL::Offset<std::uintptr_t> funcBase(a_hookAddr);

	struct Patch : Xbyak::CodeGenerator
	{
		Patch(std::size_t a_maxSize, void* a_buf, std::size_t a_callAddr) : Xbyak::CodeGenerator(a_maxSize, a_buf)
		{
			Xbyak::Label callLbl;

			jmp(ptr[rip + callLbl]);

			L(callLbl);
			dq(a_callAddr);
		}
	};

	auto trampoline = SKSE::GetTrampoline();
	auto patchBuf = trampoline->StartAlloc();
	Patch patch(trampoline->FreeSize(), patchBuf, a_funcAddr);
	trampoline->EndAlloc(patch.getSize());

	assert(patch.getSize() <= CAVE_SIZE);

	for (std::size_t i = 0; i < patch.getSize(); ++i) {
		SafeWrite8(funcBase.GetAddress() + i, patch.getCode()[i]);
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
