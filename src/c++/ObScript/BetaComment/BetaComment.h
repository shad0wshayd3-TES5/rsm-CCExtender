#pragma once

class BetaComment
{
public:
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, std::uint32_t& a_opcodeOffsetPtr);
	static void Register();

private:
	using Buffer = std::stringstream;

	BetaComment() = delete;
	BetaComment(const BetaComment&) = delete;
	BetaComment(BetaComment&&) = delete;
	~BetaComment() = delete;

	BetaComment& operator=(const BetaComment&) = delete;
	BetaComment& operator=(BetaComment&&) = delete;

	static void CPrint(const char* a_fmt, ...);
	static const char* HelpStr();
	static void Init();
	static void LogComment(const std::string& a_comment);

	static bool PrintCellCoordinates(Buffer& a_buf);
	static bool PrintCellEditorID(Buffer& a_buf);
	static bool PrintCellFormID(Buffer& a_buf);
	static bool PrintRefCoordinates(Buffer& a_buf);
	static bool PrintRefEditorID(Buffer& a_buf);
	static bool PrintRefFormID(Buffer& a_buf);
	static bool PrintSourceFile(Buffer& a_buf);
	static bool PrintTime(Buffer& a_buf);
	static bool PrintUserName(Buffer& a_buf);

	static constexpr char LONG_NAME[] = "BetaComment";
	static constexpr char SHORT_NAME[] = "BC";
	static constexpr char _DELIM{ '\t' };
	static inline std::ofstream _file{};
	static inline RE::TESObjectREFR* _ref{ nullptr };
	static inline CHAR _userName[UNLEN + 1]{ '\0' };
};
