#include "Clear.h"

#include "SKSE/API.h"


bool Clear::Exec([[maybe_unused]] const RE::SCRIPT_PARAMETER* a_paramInfo, [[maybe_unused]] RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, [[maybe_unused]] RE::TESObjectREFR* a_thisObj, [[maybe_unused]] RE::TESObjectREFR* a_containingObj, [[maybe_unused]] RE::Script* a_scriptObj, [[maybe_unused]] RE::ScriptLocals* a_locals, [[maybe_unused]] double& a_result, [[maybe_unused]] UInt32& a_opcodeOffsetPtr)
{
	auto task = SKSE::GetTaskInterface();
	task->AddUITask([]()
	{
		auto ui = RE::UI::GetSingleton();
		if (!ui) {
			return;
		}

		auto console = ui->GetMenu<RE::Console>();
		if (!console | !console->view) {
			return;
		}

		console->view->Invoke("Console.ClearHistory", 0, 0, 0);
	});

	return true;
}


void Clear::Register()
{
	auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("DumpNiUpdates");	// unused
	if (info) {
		info->functionName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpString = HelpStr();
		info->referenceFunction = false;
		info->params = 0;
		info->numParams = 0;
		info->executeFunction = &Exec;
		info->conditionFunction = 0;

		_MESSAGE("Registered console command: %s (%s)", LONG_NAME, SHORT_NAME);
	} else {
		_ERROR("Failed to register console command: %s (%s)!\n", LONG_NAME, SHORT_NAME);
	}
}


const char* Clear::HelpStr()
{
	static std::string help;
	if (help.empty()) {
		help += "\"Clear\"";
	}
	return help.c_str();
}
