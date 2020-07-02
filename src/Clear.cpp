#include "Clear.h"

bool Clear::Exec(const RE::SCRIPT_PARAMETER*, RE::SCRIPT_FUNCTION::ScriptData*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, double&, UInt32&)
{
	auto task = SKSE::GetTaskInterface();
	task->AddUITask([]() {
		auto ui = RE::UI::GetSingleton();
		auto console = ui ? ui->GetMenu<RE::Console>() : nullptr;
		auto view = console ? console->uiMovie : nullptr;
		if (view) {
			view->Invoke("Console.ClearHistory", 0, 0, 0);
		}
	});

	return true;
}

void Clear::Register()
{
	auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("DumpNiUpdates");	 // unused
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
