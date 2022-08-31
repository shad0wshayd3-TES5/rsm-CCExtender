#pragma once

class CommandPipe
{
public:
	static void InstallHooks();

protected:
	static void CompileAndRun(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_targetRef);

private:
	CommandPipe() = delete;
	CommandPipe(const CommandPipe&) = delete;
	CommandPipe(CommandPipe&&) = delete;
	~CommandPipe() = delete;

	CommandPipe& operator=(const CommandPipe&) = delete;
	CommandPipe& operator=(CommandPipe&&) = delete;

	static void CPrint(const char* a_string);
	static bool Parse(std::string& a_command, std::optional<std::string>& a_fileName);

	static inline std::ofstream _outFile;
	static inline REL::Relocation<decltype(CompileAndRun)> _CompileAndRun;
};
