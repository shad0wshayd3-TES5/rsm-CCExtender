#include "Clear.h"

bool Clear::Exec(const RE::SCRIPT_PARAMETER*, RE::SCRIPT_FUNCTION::ScriptData*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, double&, std::uint32_t&)
{
	auto task = SKSE::GetTaskInterface();
	task->AddUITask([]() {
		auto ui = RE::UI::GetSingleton();
		auto console = ui ? ui->GetMenu<RE::Console>() : nullptr;
		auto view = console ? console->uiMovie : nullptr;
		if (view) {
			view->Invoke("Console.ClearHistory", nullptr, nullptr, 0);
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
		info->params = nullptr;
		info->numParams = 0;
		info->executeFunction = &Exec;
		info->conditionFunction = nullptr;

		logger::info(FMT_STRING("Registered console command: {} ({})"), LONG_NAME, SHORT_NAME);
	} else {
		logger::error(FMT_STRING("Failed to register console command: {} ({})"), LONG_NAME, SHORT_NAME);
	}
}

const char* Clear::HelpStr()
{
	static const std::string help = []() {
		std::string str;
		str += "\"Clear\"";
		return str;
	}();

	return help.c_str();
}
