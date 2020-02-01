#pragma once

#include <fstream>
#include <optional>
#include <string>

#include "RE/Skyrim.h"


class CommandPipe
{
public:
	static void InstallHooks();
	static void Hook_CompileAndRun(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_targetRef);

private:
	static inline auto CompileAndRun_f = &CommandPipe::Hook_CompileAndRun;
	using CompileAndRun_t = function_type_t<decltype(CompileAndRun_f)>;


	CommandPipe() = delete;
	CommandPipe(const CommandPipe&) = delete;
	CommandPipe(CommandPipe&&) = delete;
	~CommandPipe() = delete;

	CommandPipe& operator=(const CommandPipe&) = delete;
	CommandPipe& operator=(CommandPipe&&) = delete;

	static void CPrint(const char* a_string);
	static bool Parse(std::string& a_command, std::optional<std::string>& a_fileName);


	static std::ofstream _outFile;
	static inline CompileAndRun_t* _CompileAndRun = 0;
};
