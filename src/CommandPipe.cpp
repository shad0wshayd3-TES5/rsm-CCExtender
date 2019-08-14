#include "CommandPipe.h"

#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/SafeWrite.h"

#include <ios>

#include "REL/Relocation.h"
#include "SKSE/API.h"


void CommandPipe::InstallHooks()
{
	// E8 ? ? ? ? 48 89 7C 24 60 48 8B CF
	constexpr std::uintptr_t FUNC_ADDR = 0x008DAE20;	// 1_5_80

	REL::Offset<std::uintptr_t> hookPoint(FUNC_ADDR + 0xE2);
	auto offset = reinterpret_cast<std::int32_t*>(hookPoint.GetAddress() + 1);
	auto nextOp = hookPoint.GetAddress() + 5;
	_Invoke = reinterpret_cast<Invoke_t*>(nextOp + *offset);
	g_branchTrampoline.Write5Call(hookPoint.GetAddress(), unrestricted_cast<std::uintptr_t>(&CommandPipe::Hook_Invoke));

	_MESSAGE("Installed hooks for class (%s)", typeid(CommandPipe).name());
}


void CommandPipe::Hook_Invoke(RE::Script* a_script, void* a_arg2, RE::Script::InvokeType a_type, RE::TESObjectREFR* a_targetRef)
{
	auto cmd = a_script->GetCommand();
	std::optional<std::string> fileName;
	if (!Parse(cmd, fileName)) {
		return;
	}

	if (fileName) {
		a_script->SetCommand(cmd);
		_outFile.open(*fileName);
	}
	
	_Invoke(a_script, a_arg2, a_type, a_targetRef);

	if (_outFile.is_open()) {
		auto console = RE::ConsoleManager::GetSingleton();
		_outFile << console->buffer.c_str();
		_outFile.close();
	}
}


void CommandPipe::CPrint(const char* a_string)
{
	std::string str((a_string ? a_string : ""));
	auto task = SKSE::GetTaskInterface();
	task->AddTask([str]()
	{
		auto console = RE::ConsoleManager::GetSingleton();
		if (console) {
			console->Print(str.c_str());
		}
	});
}


bool CommandPipe::Parse(std::string& a_command, std::optional<std::string>& a_fileName)
{
	bool lastWasSpace = false;
	for (std::size_t i = 0; i < a_command.length(); ++i) {
		switch (a_command[i]) {
		case '>':
			if (!lastWasSpace) {
				break;
			} else if (i + 1 >= a_command.length() || a_command[i + 1] != ' ') {
				CPrint("ERROR: Expected space following pipe");
				return false;
			}

			for (std::size_t j = i + 2; j < a_command.length(); ++j) {
				switch (a_command[j]) {
				case '\"':
					{
						if (!std::isalnum(a_command[++j])) {
							CPrint("ERROR: Expected file name");
							return false;
						}

						std::size_t k = j;
						do {
							++k;
						} while (k < a_command.length() && a_command[k] != '\"');
						if (k >= a_command.length()) {
							CPrint("ERROR: Expected '\"' before end of line");
							return false;
						} else {
							std::string result(a_command, j, k - j);
							a_command.erase(i - 1);
							a_fileName = std::make_optional(std::move(result));
							return true;
						}
					}
					break;
				default:
					if (std::isalnum(a_command[j])) {
						std::size_t k = j;
						do {
							++k;
						} while (k < a_command.length() && std::isalnum(a_command[k]));
						if (k < a_command.length()) {
							CPrint("ERROR: Expected end of line");
							return false;
						} else {
							std::string result(a_command, j, k - j);
							a_command.erase(i - 1);
							a_fileName = std::make_optional(std::move(result));
							return true;
						}
					}
					break;
				}
			}

			CPrint("ERROR: Expected expression following pipe");
			return false;
		case ' ':
			lastWasSpace = true;
			break;
		default:
			lastWasSpace = false;
			break;
		}
	}
	return true;
}


decltype(CommandPipe::_outFile) CommandPipe::_outFile;
