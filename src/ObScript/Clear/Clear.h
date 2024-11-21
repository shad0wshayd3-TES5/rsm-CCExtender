#pragma once

class Clear
{
public:
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, std::uint32_t& a_opcodeOffsetPtr);
	static void Register();

private:
	Clear() = delete;
	Clear(const Clear&) = delete;
	Clear(Clear&&) = delete;
	~Clear() = delete;

	Clear& operator=(const Clear&) = delete;
	Clear& operator=(Clear&&) = delete;

	static const char* HelpStr();

	static constexpr char LONG_NAME[] = "Clear";
	static constexpr char SHORT_NAME[] = "cls";
};
