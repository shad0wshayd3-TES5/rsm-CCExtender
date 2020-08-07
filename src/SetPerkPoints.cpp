#include "SetPerkPoints.h"

bool SetPerkPoints::Exec(const RE::SCRIPT_PARAMETER*, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, double&, std::uint32_t&)
{
	auto num = a_scriptData->GetIntegerChunk()->GetInteger();
	if (num < 0) {
		CPrint("> [%s] ERROR: Cannot set perk points to a negative number (%i)", LONG_NAME, num);
		return true;
	} else if (num > std::numeric_limits<std::int8_t>::max()) {
		CPrint("> [%s] ERROR: Cannot set perk points greater than %i (%i)", LONG_NAME, std::numeric_limits<std::int8_t>::max(), num);
		return true;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (!player) {
		CPrint("> [%s] ERROR: Player is NONE", LONG_NAME);
		return true;
	}

	player->perkCount = static_cast<std::int8_t>(num);
	CPrint("> [%s] Player now has %i perk points", LONG_NAME, num);

	return true;
}

void SetPerkPoints::Register()
{
	using Type = RE::SCRIPT_PARAM_TYPE;

	auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("Timing");  // unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "Integer", Type::kInt, 1 }
		};

		info->functionName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpString = HelpStr();
		info->referenceFunction = false;
		info->SetParameters(params);
		info->executeFunction = Exec;
		info->conditionFunction = nullptr;

		logger::info(FMT_STRING("Registered console command: {} ({})"), LONG_NAME, SHORT_NAME);
	} else {
		logger::error(FMT_STRING("Failed to register console command: {} ({})"), LONG_NAME, SHORT_NAME);
	}
}

const char* SetPerkPoints::HelpStr()
{
	static const std::string help = []() {
		std::string str;
		str += "<setperkpoints> \" \" <numperkpoints>";
		str += "\n\t<setperkpoints> ::= \"SetPerkPoints\" | \"SPP\"";
		str += "\n\t<numperkpoints> ::= <integer> ; The number of perk points";
		return str;
	}();

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
