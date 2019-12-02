#include "Help.h"

#include "FormStringTable.h"
#include "Utility.h"


namespace
{
	void StopWatch::Start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}


	void StopWatch::TimeStamp()
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - _start;
		_MESSAGE("Milestone: %gs", diff.count());
	}


	void kmp_table(const std::string_view& W, std::vector<std::size_t>& T)
	{
		std::size_t pos = 1;
		std::size_t cnd = 0;

		T[0] = NPOS;

		while (pos < W.size()) {
			if (W[pos] == W[cnd]) {
				T[pos] = T[cnd];
			} else {
				T[pos] = cnd;
				cnd = T[cnd];
				while (cnd != NPOS && W[pos] != W[cnd]) {
					cnd = T[cnd];
				}
			}
			++pos;
			++cnd;
		}

		T[pos] = cnd;
	}


	bool kmp_search(const std::string_view& S, const std::string_view& W)
	{
		std::size_t j = 0;
		std::size_t k = 0;
		std::vector<std::size_t> T(W.size() + 1);
		kmp_table(W, T);

		while (j < S.size()) {
			if (W[k] == S[j]) {
				++j;
				++k;
				if (k == W.size()) {
					return true;
				}
			} else {
				k = T[k];
				if (k == NPOS) {
					++j;
					++k;
				}
			}
		}

		return false;
	}
}


bool Help::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	MatchString matchString;
	Filter filter;
	FormType formType;
	ParseParams(a_scriptData, matchString, filter, formType);

	if (!matchString || matchString->length() == 0) {
		CPrint(HelpStr());
		return true;
	}

	_needle = std::move(*matchString);
	for (auto& ch : _needle) {
		ch = std::tolower(ch);
	}

#if _DEBUG
	_stopWatch.Start();
#endif

	if (!filter || *filter == FilterType::kAll || *filter == FilterType::kFunctions) {
		EnumerateFunctions();
	}

	if (!filter || *filter == FilterType::kAll || *filter == FilterType::kSettings) {
		EnumerateSettings();
	}

	if (!filter || *filter == FilterType::kAll || *filter == FilterType::kGlobals) {
		EnumerateGlobals();
	}

	if (!filter || *filter == FilterType::kAll || *filter == FilterType::kForms) {
		EnumerateForms(formType);
	}

#if _DEBUG
	_stopWatch.TimeStamp();
#endif

	return true;
}


void Help::Register()
{
	using Type = RE::SCRIPT_PARAMETER::Type;

	auto info = RE::CommandInfo::LocateConsoleCommand("Help");
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "String (Optional)", Type::kString, 1 },
			{ "Integer (Optional)", Type::kInteger, 1 },
			{ "String (Optional)", Type::kString, 1 }
		};

		info->longName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpText = HelpStr();
		info->isRefRequired = false;
		info->SetParameters(params);
		info->execute = &Exec;
		info->eval = 0;

		_MESSAGE("Registered console command: %s (%s)", LONG_NAME, SHORT_NAME);
	} else {
		_ERROR("Failed to register console command: %s (%s)!\n", LONG_NAME, SHORT_NAME);
	}
}


Help::FormInfo::FormInfo(RE::TESForm* a_form) :
	_editorID(""),
	_fullName(""),
	_formID(a_form->formID),
	_formType(a_form->formType)
{
	_editorID = safe_cstr(a_form->GetEditorID());
	if (!_editorID.empty()) {
		_editorID.push_back(' ');
	}

	_fullName = GetFullName(a_form);
	if (!_fullName.empty()) {
		_fullName = " '" + _fullName + '\'';
	}
}


bool Help::FormInfo::operator<(const FormInfo& a_rhs) const
{
	if (_formType != a_rhs._formType) {
		return _formType < a_rhs._formType;
	}

	return _formID < a_rhs._formID;
}


RE::FormType Help::FormInfo::GetFormType() const
{
	return _formType;
}


void Help::FormInfo::Print() const
{
	auto strTable = FormStringTable::GetSingleton();
	auto typeString = strTable->MapFormTypeToString(_formType);
	CPrint("%s: %s(%08X)%s", typeString.data(), _editorID.c_str(), _formID, _fullName.c_str());
}


Help::Setting::Setting(const RE::Setting* a_setting) :
	_setting(a_setting),
	_name(safe_cstr(a_setting->GetName()))
{
	assert(_setting);
}


bool Help::Setting::operator<(const Setting& a_rhs) const
{
	return _stricmp(_name.c_str(), a_rhs._name.c_str()) < 0;
}


void Help::Setting::Print() const
{
	using Type = RE::Setting::Type;

	switch (_setting->GetType()) {
	case Type::kBool:
		CPrint("%s = %s", _name.c_str(), (_setting->GetBool() ? "True" : "False"));
		break;
	case Type::kFloat:
		CPrint("%s = %0.2f", _name.c_str(), _setting->GetFloat());
		break;
	case Type::kSignedInteger:
		CPrint("%s = %i", _name.c_str(), _setting->GetSInt());
		break;
	case Type::kColor:
		{
			auto color = _setting->GetColor();
			CPrint("%s = R:%u G:%u B:%u A:%u)", _name.c_str(), color.red, color.green, color.blue, color.alpha);
		}
		break;
	case Type::kString:
		CPrint("%s = %s", _name.c_str(), _setting->GetString());
		break;
	case Type::kUnsignedInteger:
		CPrint("%s = %u", _name.c_str(), _setting->GetUInt());
		break;
	default:
		CPrint("%s = <UNKNOWN>", _name.c_str());
		break;
	}
}


const char* Help::HelpStr()
{
	static std::string help;
	if (help.empty()) {
		help += "\"Help\" <expr>";
		help += "\n\t<expr> ::= <empty> | \" \" <matchstring> | \" \" <matchstring> \" \" <filter> | \" \" <matchstring> \" \" <filter> \" \" <form-type>";
		help += "\n\t<matchstring> ::= <string> ; The string to filter results with";
		help += "\n\t<filter> ::= <integer>";
		help += "\n\t\t; 0 - All";
		help += "\n\t\t; 1 - Functions";
		help += "\n\t\t; 2 - Settings";
		help += "\n\t\t; 3 - Globals";
		help += "\n\t\t; 4 - Forms";
		help += "\n\t<form-type> ::= <string> ; The form type to filter form results with";
	}
	return help.c_str();
}


void Help::CPrint(const char* a_fmt, ...)
{
	auto console = RE::ConsoleManager::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->VPrint(a_fmt, args);
		va_end(args);
	}
}


void Help::ParseParams(RE::CommandInfo::ScriptData* a_scriptData, MatchString& a_matchString, Filter& a_filter, FormType& a_formType)
{
	RE::CommandInfo::StringChunk* strChunk = 0;
	RE::CommandInfo::IntegerChunk* intChunk = 0;

	if (a_scriptData->numParams < 1) {
		return;
	}

	strChunk = a_scriptData->GetStringChunk();
	a_matchString = std::make_optional(strChunk->GetString());
	if (a_scriptData->numParams < 2) {
		return;
	}

	intChunk = strChunk->GetNext()->AsInteger();
	a_filter = std::make_optional(static_cast<FilterType>(intChunk->GetInteger()));
	if (a_scriptData->numParams < 3) {
		return;
	}

	strChunk = intChunk->GetNext()->AsString();
	a_formType = std::make_optional(strChunk->GetString());
}


bool Help::Match(const std::string_view& a_haystack)
{
	if (a_haystack.length() == 0 || _needle.length() > a_haystack.length()) {
		return false;
	}

	std::string haystack(a_haystack);
	for (auto& ch : haystack) {
		ch = std::tolower(ch);
	}

	return kmp_search(haystack, _needle);
}


std::string_view Help::GetFullName(RE::TESForm* a_form)
{
	std::string_view result("");

	switch (a_form->formType) {
	case RE::FormType::Dialogue:
		break;
	default:
		{
			auto fullName = a_form->As<RE::TESFullName*>();
			if (fullName) {
				auto cstr = fullName->GetFullName();
				if (cstr) {
					result = cstr;
				}
			}
		}
		break;
	}

	return result;
}


void Help::EnumerateFunctions()
{
	CPrint("----CONSOLE COMMANDS--------------------");
	auto iter = RE::CommandInfo::GetFirstConsoleCommand();
	EnumerateFunctions(iter, iter + RE::CommandInfo::Commands::kConsoleCommandsEnd);

	CPrint("----SCRIPT FUNCTIONS--------------------");
	iter = RE::CommandInfo::GetFirstScriptCommand();
	EnumerateFunctions(iter, iter + RE::CommandInfo::Commands::kScriptCommandsEnd);
}


void Help::EnumerateFunctions(RE::CommandInfo* a_beg, RE::CommandInfo* a_end)
{
	std::string_view commandName;
	std::string_view longNameV;
	std::string_view shortNameV;
	for (auto it = a_beg; it != a_end; ++it) {
		longNameV = safe_cstr(it->longName);
		shortNameV = safe_cstr(it->shortName);
		for (std::size_t j = 0; j < 2; ++j) {
			switch (j) {
			case 0:
				commandName = longNameV;
				break;
			case 1:
				commandName = shortNameV;
				break;
			default:
				commandName = "";
				break;
			}

			if (Match(commandName)) {
				std::string shortName(shortNameV);
				if (!shortName.empty()) {
					shortName = " (" + shortName + ")";
				}

				std::string helpText(safe_cstr(it->helpText));
				if (!helpText.empty()) {
					helpText = " -> " + helpText;
				}

				CPrint("%s%s%s", longNameV.data(), shortName.c_str(), helpText.c_str());
				break;
			}
		}
	}
}


void Help::EnumerateSettings()
{
	CPrint("----SETTINGS----------------------------");

	std::set<Setting> settings;

	auto gmst = RE::GameSettingCollection::GetSingleton();
	for (auto& setting : gmst->settings) {
		if (Match(setting.key)) {
			settings.emplace(setting.mapped);
		}
	}

	auto ini = RE::INISettingCollection::GetSingleton();
	auto iniPref = RE::INIPrefSettingCollection::GetSingleton();
	for (auto& setting : ini->settings) {
		if (Match(setting->GetName())) {
			auto pref = iniPref->GetSetting(setting->GetName());
			if (pref) {
				settings.emplace(pref);
			} else {
				settings.emplace(setting);
			}
		}
	}

	for (auto& setting : settings) {
		setting.Print();
	}
}


void Help::EnumerateGlobals()
{
	CPrint("----GLOBAL VARIABLES--------------------");

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	auto& globals = dataHandler->GetFormArray<RE::TESGlobal>();
	std::string editorID;
	for (auto& global : globals) {
		editorID = safe_cstr(global->GetEditorID());
		if (Match(editorID)) {
			CPrint("%s = %0.2f", editorID.c_str(), global->value);
		}
	}
}


void Help::EnumerateForms(const FormType& a_formType)
{
	CPrint("----OTHER FORMS--------------------");

	auto formInfoSet = GatherFormInfo(a_formType);
	if (formInfoSet) {
		for (auto& form : *formInfoSet) {
			form.Print();
		}
	}
}


auto Help::GatherFormInfo(const FormType& a_formType)
	-> std::optional<std::set<FormInfo>>
{
	auto lookupInfo = RE::GlobalLookupInfo::GetSingleton();
	RE::BSReadLockGuard locker(lookupInfo->formIDsLock);
	if (!lookupInfo->formIDs) {
		return std::nullopt;
	}
	auto& formMap = *lookupInfo->formIDs;

	auto strTable = FormStringTable::GetSingleton();
	auto formFilter = a_formType ? strTable->MapStringToFormType(*a_formType) : RE::FormType::None;

	std::set<FormInfo> formInfoSet;
	std::string fullName;
	std::string editorID;
	std::string_view name;
	for (auto& elem : formMap) {
		auto form = elem.second;
		if (!form) {
			continue;
		} else if (a_formType && form->formType != formFilter) {
			continue;
		}

		editorID = safe_cstr(form->GetEditorID());
		fullName = GetFullName(form);

		for (std::size_t j = 0; j < 2; ++j) {
			switch (j) {
			case 0:
				name = editorID;
				break;
			case 1:
				name = fullName;
				break;
			default:
				name = "";
				break;
			}

			if (Match(name)) {
				formInfoSet.emplace(form);
				break;
			}
		}
	}

	return std::make_optional(std::move(formInfoSet));
}


decltype(Help::_needle) Help::_needle;
#if _DEBUG
decltype(Help::_stopWatch) Help::_stopWatch;
#endif
