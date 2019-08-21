#include "BetaComment.h"

#include <timezoneapi.h>

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include "Settings.h"
#include "Utility.h"


bool BetaComment::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	if (!_file.is_open()) {
		CPrint("> [BetaComment] ERROR: Failed to open output file");
		return true;
	}

	auto selectedRefHandle = RE::Console::GetSelectedRef();
	RE::TESObjectREFRPtr selectedRef;
	RE::TESObjectREFR::LookupByHandle(selectedRefHandle, selectedRef);
	if (!selectedRef) {
		CPrint("> [BetaComment] ERROR: No selected reference");
		return true;
	}

	auto strChunk = a_scriptData->GetStringChunk();
	auto comment = strChunk->GetString();
	_ref = selectedRef.get();
	LogComment(comment);
	_ref = 0;

	return true;
}


void BetaComment::Register()
{
	using Type = RE::SCRIPT_PARAMETER::Type;

	Init();

	auto info = RE::CommandInfo::LocateConsoleCommand("BetaComment");  // Unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "Comment", Type::kString, 0 }
		};
		info->longName = "BetaComment";
		info->shortName = "BC";
		info->helpText = HelpStr();
		info->isRefRequired = false;
		info->SetParameters(params);
		info->execute = &Exec;
		info->eval = 0;
		info->flags = 0;

		_MESSAGE("Registered console command: %s (%s)", info->longName, info->shortName);
	} else {
		_ERROR("Failed to register console command!\n");
	}
}


void BetaComment::CPrint(const char* a_fmt, ...)
{
	auto console = RE::ConsoleManager::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->VPrint(a_fmt, args);
		va_end(args);
	}
}


const char* BetaComment::HelpStr()
{
	static std::string help;
	if (help.empty()) {
		help += "<betacomment> <comment>";
		help += "\n\t<betacomment> ::= \"BetaComment\" | \"BC\"";
		help += "\n\t<comment> ::= <string> ; The comment string";
	}
	return help.c_str();
}


void BetaComment::Init()
{
	constexpr auto USERNAME_SIZE = std::extent<decltype(_userName)>::value;

	_file.open(Settings::betaCommentFileName.c_str());

	DWORD tmpSize = USERNAME_SIZE;
	if (!GetUserName(_userName, &tmpSize)) {
		_ERROR("Failed to get username with error code (%i)", GetLastError());
		strcpy_s(_userName, USERNAME_SIZE, "SEE-LOG-FOR-ERROR");
	}
}


void BetaComment::LogComment(const std::string& a_comment)
{
	Buffer line;
	Buffer tmp;

	if (!PrintUserName(line)) {
		CPrint("> [BetaComment] ERROR: Failed to get username");
		return;
	}
	line << ':' << _DELIM;

	line << '(';
	if (!PrintTime(line)) {
		CPrint("> [BetaComment] ERROR: Failed to get local time");
		return;
	}
	line << ')' << _DELIM;

	if (!PrintSourceFile(line)) {
		line << "NO-SOURCE";
	}
	line << _DELIM;

	line << '[';
	if (!PrintCellFormID(line)) {
		CPrint("> [BetaComment] ERROR: Failed to get cell form ID");
		return;
	}

	if (PrintCellEditorID(tmp)) {
		line << ": " << tmp.str();
		tmp.str("");
	}

	if (PrintCellCoordinates(tmp)) {
		line << " (" << tmp.str() << ')';
		tmp.str("");
	}
	line << ']' << _DELIM;

	line << '[';
	if (!PrintRefFormID(line)) {
		CPrint("> [BetaComment] ERROR: Failed to get selected ref form ID");
		return;
	}
	line << ": ";

	if (PrintRefEditorID(tmp)) {
		line << tmp.str() << ' ';
		tmp.str("");
	}

	line << '(';
	if (!PrintRefCoordinates(line)) {
		CPrint("> [BetaComment] ERROR: Failed to get selected ref coordinates");
		return;
	}
	line << ")]" << _DELIM;

	line << '\"' << a_comment << '\"';

	_file << line.str() << '\n';
	_file.flush();
}


bool BetaComment::PrintCellCoordinates(Buffer& a_buf)
{
	auto cell = _ref->GetParentCell();
	if (!cell) {
		return false;
	}

	auto coordinates = cell->GetCoordinates();
	if (!coordinates) {
		return false;
	}

	a_buf << coordinates->x << ", " << coordinates->y;

	return true;
}


bool BetaComment::PrintCellEditorID(Buffer& a_buf)
{
	auto cell = _ref->GetParentCell();
	if (!cell) {
		return false;
	}

	std::string editorID = safe_cstr(cell->GetEditorID());
	if (editorID.empty()) {
		return false;
	}

	a_buf << editorID;

	return true;
}


bool BetaComment::PrintCellFormID(Buffer& a_buf)
{
	char formID[] = "0xDEADBEEF";
	auto cell = _ref->GetParentCell();
	std::snprintf(formID, sizeof(formID), "0x%08X", cell->formID);
	a_buf << formID;
	return true;
}


bool BetaComment::PrintRefCoordinates(Buffer& a_buf)
{
	a_buf << std::floor(_ref->pos.x) << ", " << std::floor(_ref->pos.y) << ", " << std::floor(_ref->pos.z);
	return true;
}


bool BetaComment::PrintRefEditorID(Buffer& a_buf)
{
	std::string editorID = safe_cstr(_ref->GetEditorID());
	if (editorID.empty()) {
		return false;
	}

	a_buf << editorID;

	return true;
}


bool BetaComment::PrintRefFormID(Buffer& a_buf)
{
	char formID[] = "0xDEADBEEF";
	std::snprintf(formID, sizeof(formID), "0x%08X", _ref->formID);
	a_buf << formID;
	return true;
}


bool BetaComment::PrintSourceFile(Buffer& a_buf)
{
	auto source = _ref->GetFinalSourceFile();
	if (!source) {
		return false;
	}

	a_buf << source->name;

	return true;
}


bool BetaComment::PrintTime(Buffer& a_buf)
{
	auto time = std::time(0);
	std::tm localTime;
	auto err = gmtime_s(&localTime, &time);
	if (err) {
		return false;
	}

	a_buf << std::put_time(&localTime, "%F %T GMT");

	return true;
}


bool BetaComment::PrintUserName(Buffer& a_buf)
{
	a_buf << _userName;
	return true;
}


decltype(BetaComment::_file) BetaComment::_file;
decltype(BetaComment::_userName) BetaComment::_userName = { 0 };
