#include "Clear.h"

#include "SKSE/API.h"


bool Clear::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	auto task = SKSE::GetTaskInterface();
	task->AddUITask([]()
	{
		auto mm = RE::MenuManager::GetSingleton();
		auto uiStr = RE::UIStringHolder::GetSingleton();
		if (!mm || !uiStr) {
			return;
		}

		auto console = mm->GetMenu<RE::Console>(uiStr->console);
		if (!console | !console->view) {
			return;
		}

		console->view->Invoke("Console.ClearHistory", 0, 0, 0);
	});

	return true;
}


void Clear::Register()
{
	auto info = RE::CommandInfo::LocateConsoleCommand("DumpNiUpdates");	// unused
	if (info) {
		info->longName = "Clear";
		info->shortName = "";
		info->helpText = "\"Clear\"";
		info->isRefRequired = false;
		info->params = 0;
		info->numParams = 0;
		info->execute = &Exec;
		info->eval = 0;

		_MESSAGE("Registered console command: %s (%s)", info->longName, info->shortName);
	} else {
		_ERROR("Failed to register console command!\n");
	}
}
