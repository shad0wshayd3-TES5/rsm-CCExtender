#pragma once

#include <fstream>
#include <optional>
#include <string>

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


class CommandPipe
{
public:
	static void InstallHooks();
	static void Hook_CompileAndRun(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_targetRef);

private:
	using CompileAndRun_t = decltype(&CommandPipe::Hook_CompileAndRun);


	CommandPipe() = delete;
	CommandPipe(const CommandPipe&) = delete;
	CommandPipe(CommandPipe&&) = delete;
	~CommandPipe() = delete;

	CommandPipe& operator=(const CommandPipe&) = delete;
	CommandPipe& operator=(CommandPipe&&) = delete;

	static void CPrint(const char* a_string);
	static bool Parse(std::string& a_command, std::optional<std::string>& a_fileName);


	static std::ofstream _outFile;
	static inline REL::Function<CompileAndRun_t> _CompileAndRun;
};
