#include "Hooks.h"

#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/SafeWrite.h"
#include "xbyak/xbyak.h"

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
				Patch(void* a_buf, std::size_t a_callAddr) : Xbyak::CodeGenerator(1024, a_buf)
				{
					Xbyak::Label callLbl;

					jmp(ptr[rip + callLbl]);

					L(callLbl);
					dq(a_callAddr);
				}
			};

			void* patchBuf = g_localTrampoline.StartAlloc();
			Patch patch(patchBuf, a_funcAddr);
			g_localTrampoline.EndAlloc(patch.getCurr());

			assert(patch.getSize() <= CAVE_SIZE);

			for (std::size_t i = 0; i < patch.getSize(); ++i) {
				SafeWrite8(funcBase.GetAddress() + i, patch.getCode()[i]);
			}
		}


		const char* TESFormEx::Hook_GetEditorID()
		{
			auto it = _idMap.find(formID);
			return it != _idMap.end() ? it->second->c_str() : "";
		}


		bool TESFormEx::Hook_SetEditorID(const char* a_str)
		{
			if (a_str) {
				_idMap[formID].reset(new std::string(a_str));
			}
			return true;
		}


		void TESFormEx::InstallHooks()
		{
			WritePatch(0x000F6E80, unrestricted_cast<std::uintptr_t>(&Hook_GetEditorID));	// 1_5_80
			WritePatch(0x000F72E0, unrestricted_cast<std::uintptr_t>(&Hook_SetEditorID));	// 1_5_80
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
