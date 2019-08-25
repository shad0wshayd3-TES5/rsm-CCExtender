#include "SetPerkPoints.h"


bool SetPerkPoints::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	auto num = a_scriptData->GetIntegerChunk()->GetInteger();
	if (num < 0) {
		CPrint("> [%s] ERROR: Cannot set perk points to a negative number (%i)", LONG_NAME, num);
		return true;
	} else if (num > std::numeric_limits<UInt8>::max()) {
		CPrint("> [%s] ERROR: Cannot set perk points greater than 255 (%i)", LONG_NAME, num);
		return true;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (!player) {
		CPrint("> [%s] ERROR: Player is NONE", LONG_NAME);
		return true;
	}

	player->numPerkPoints = static_cast<UInt8>(num);
	CPrint("> [%s] Player now has %i perk points", LONG_NAME, num);

	return true;
}


void SetPerkPoints::Register()
{
	using Type = RE::SCRIPT_PARAMETER::Type;

	auto info = RE::CommandInfo::LocateConsoleCommand("Timing");	// unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "Integer", Type::kInteger, 1 }
		};

		info->longName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpText = HelpStr();
		info->isRefRequired = false;
		info->SetParameters(params);
		info->execute = &Exec;
		info->eval = 0;

		_MESSAGE("Registered console command: %s (%s)", info->longName, info->shortName);
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
	auto console = RE::ConsoleManager::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->VPrint(a_fmt, args);
		va_end(args);
	}
}
