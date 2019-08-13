#pragma once

#include "RE/Skyrim.h"


class Clear
{
public:
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr);
	static void Register();

private:
	Clear() = delete;
	Clear(const Clear&) = delete;
	Clear(Clear&&) = delete;
	~Clear() = delete;

	Clear& operator=(const Clear&) = delete;
	Clear& operator=(Clear&&) = delete;
};
