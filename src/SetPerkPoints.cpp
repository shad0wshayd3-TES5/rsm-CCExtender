#include "SetPerkPoints.h"


bool SetPerkPoints::Exec([[maybe_unused]] const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, [[maybe_unused]] RE::TESObjectREFR* a_thisObj, [[maybe_unused]] RE::TESObjectREFR* a_containingObj, [[maybe_unused]] RE::Script* a_scriptObj, [[maybe_unused]] RE::ScriptLocals* a_locals, [[maybe_unused]] double& a_result, [[maybe_unused]] UInt32& a_opcodeOffsetPtr)
{
	auto num = a_scriptData->GetIntegerChunk()->GetInteger();
	if (num < 0) {
		CPrint("> [%s] ERROR: Cannot set perk points to a negative number (%i)", LONG_NAME, num);
		return true;
	} else if (num > std::numeric_limits<SInt8>::max()) {
		CPrint("> [%s] ERROR: Cannot set perk points greater than %i (%i)", LONG_NAME, std::numeric_limits<SInt8>::max(), num);
		return true;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (!player) {
		CPrint("> [%s] ERROR: Player is NONE", LONG_NAME);
		return true;
	}

	player->perkCount = static_cast<SInt8>(num);
	CPrint("> [%s] Player now has %i perk points", LONG_NAME, num);

	return true;
}


void SetPerkPoints::Register()
{
	using Type = RE::SCRIPT_PARAM_TYPE;

	auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("Timing");	// unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "Integer", Type::kInt, 1 }
		};

		info->functionName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpString = HelpStr();
		info->referenceFunction = false;
		info->SetParameters(params);
		info->executeFunction = &Exec;
		info->conditionFunction = 0;

		_MESSAGE("Registered console command: %s (%s)", LONG_NAME, SHORT_NAME);
	} else {
		_ERROR("Failed to register console command!\n");
	}
}


const char* SetPerkPoints::HelpStr()
{
	static std::string help;
	if (help.empty()) {
		help += "<setperkpoints> \" \" <numperkpoints>";
		help += "\n\t<setperkpoints> ::= \"SetPerkPoints\" | \"SPP\"";
		help += "\n\t<numperkpoints> ::= <integer> ; The number of perk points";
	}
	return help.c_str();
}


void SetPerkPoints::CPrint(const char* a_fmt, ...)
{
	auto console = RE::ConsoleLog::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->VPrint(a_fmt, args);
		va_end(args);
	}
}
