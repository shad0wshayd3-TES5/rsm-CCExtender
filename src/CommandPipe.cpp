#include "CommandPipe.h"

#include <ios>
#include <typeinfo>

#include "REL/Relocation.h"
#include "SKSE/API.h"
#include "SKSE/Trampoline.h"


void CommandPipe::InstallHooks()
{
	// E8 ? ? ? ? 48 89 7C 24 60 48 8B CF
	constexpr std::uintptr_t FUNC_ADDR = 0x008DAE20;	// 1_5_97

	REL::Offset<std::uintptr_t> funcBase(FUNC_ADDR);
	auto trampoline = SKSE::GetTrampoline();
	_CompileAndRun = trampoline->Write5CallEx<CompileAndRun_t*>(funcBase.GetAddress() + 0xE2, CompileAndRun_f);

	_MESSAGE("Installed hooks for class (%s)", typeid(CommandPipe).name());
}


void CommandPipe::Hook_CompileAndRun(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_targetRef)
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
	
	_CompileAndRun(a_script, a_compiler, a_name, a_targetRef);

	if (_outFile.is_open()) {
		auto console = RE::ConsoleLog::GetSingleton();
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
		auto console = RE::ConsoleLog::GetSingleton();
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
