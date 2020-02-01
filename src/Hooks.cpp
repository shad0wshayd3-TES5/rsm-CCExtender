#include "Hooks.h"

#include "skse64_common/SafeWrite.h"
#include "xbyak/xbyak.h"

#include "SKSE/API.h"
#include "SKSE/Trampoline.h"

#include "CommandPipe.h"


namespace Hooks
{
	namespace
	{
		void TESFormEx::WritePatch(std::uintptr_t a_hookAddr, std::uintptr_t a_funcAddr)
		{
			constexpr std::size_t CAVE_SIZE = 0x20;

			REL::Offset<std::uintptr_t> funcBase(a_hookAddr);

			struct Patch : Xbyak::CodeGenerator
			{
				Patch(std::uintptr_t a_maxSize, void* a_buf, std::size_t a_callAddr) : Xbyak::CodeGenerator(a_maxSize, a_buf)
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


		const char* TESFormEx::Hook_GetEditorID()
		{
			Locker locker(_lock);
			auto it = _idMap.find(formID);
			return it != _idMap.end() ? it->second->c_str() : "";
		}


		bool TESFormEx::Hook_SetEditorID(const char* a_str)
		{
			if (a_str) {
				Locker locker(_lock);
				_idMap[formID].reset(new std::string(a_str));
			}
			return true;
		}


		void TESFormEx::InstallHooks()
		{
			WritePatch(0x000F6E80, unrestricted_cast<std::uintptr_t>(&Hook_GetEditorID));	// 1_5_97
			WritePatch(0x000F72E0, unrestricted_cast<std::uintptr_t>(&Hook_SetEditorID));	// 1_5_97
			_DMESSAGE("Installed hooks for (%s)", typeid(TESFormEx).name());
		}


		decltype(TESFormEx::_idMap) TESFormEx::_idMap;
	}


	void Install()
	{
		TESFormEx::InstallHooks();
		CommandPipe::InstallHooks();
		_MESSAGE("Installed all hooks");
	}
}
