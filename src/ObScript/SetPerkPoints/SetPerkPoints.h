#pragma once

class SetPerkPoints
{
public:
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, std::uint32_t& a_opcodeOffsetPtr);
	static void Register();

private:
	SetPerkPoints() = delete;
	SetPerkPoints(const SetPerkPoints&) = delete;
	SetPerkPoints(SetPerkPoints&&) = delete;
	~SetPerkPoints() = delete;

	SetPerkPoints& operator=(const SetPerkPoints&) = delete;
	SetPerkPoints& operator=(SetPerkPoints&&) = delete;

	static const char* HelpStr();
	static void CPrint(const char* a_fmt, ...);

	static constexpr char LONG_NAME[] = "SetPerkPoints";
	static constexpr char SHORT_NAME[] = "SPP";
};
