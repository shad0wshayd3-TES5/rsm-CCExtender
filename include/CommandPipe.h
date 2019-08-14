#pragma once

#include <fstream>
#include <optional>
#include <string>

#include "RE/Skyrim.h"


class CommandPipe
{
public:
	static void InstallHooks();
	static void Hook_Invoke(RE::Script* a_script, void* a_arg2, RE::Script::InvokeType a_type, RE::TESObjectREFR* a_targetRef);

private:
	using Invoke_t = function_type_t<decltype(&CommandPipe::Hook_Invoke)>;


	CommandPipe() = delete;
	CommandPipe(const CommandPipe&) = delete;
	CommandPipe(CommandPipe&&) = delete;
	~CommandPipe() = delete;

	CommandPipe& operator=(const CommandPipe&) = delete;
	CommandPipe& operator=(CommandPipe&&) = delete;

	static void CPrint(const char* a_string);
	static bool Parse(std::string& a_command, std::optional<std::string>& a_fileName);


	static std::ofstream _outFile;
	static inline Invoke_t* _Invoke = 0;
};
