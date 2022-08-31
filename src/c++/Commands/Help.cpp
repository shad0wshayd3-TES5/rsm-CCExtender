#include "Help.h"

#include "EditorIDCache.h"
#include "FormStringTable.h"

bool Help::Exec(const RE::SCRIPT_PARAMETER*, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, double&, std::uint32_t&)
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
		ch = static_cast<char>(std::tolower(ch));
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
	using Type = RE::SCRIPT_PARAM_TYPE;

	auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("Help");
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "String (Optional)", Type::kChar, 1 },
			{ "Integer (Optional)", Type::kInt, 1 },
			{ "String (Optional)", Type::kChar, 1 }
		};

		info->functionName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpString = HelpStr();
		info->referenceFunction = false;
		info->SetParameters(params);
		info->executeFunction = Exec;
		info->conditionFunction = nullptr;

		logger::info(FMT_STRING("Registered console command: {} ({})"), LONG_NAME, SHORT_NAME);
	} else {
		logger::error(FMT_STRING("Failed to register console command: {} ({})"), LONG_NAME, SHORT_NAME);
	}
}

Help::FormInfo::FormInfo(RE::TESForm* a_form) :
	_pluginName(""),
	_editorID(""),
	_fullName(""),
	_formID(a_form->formID),
	_formType(a_form->GetFormType())
{
	assert(a_form != nullptr);

	auto plugin = a_form->GetDescriptionOwnerFile();
	if (plugin) {
		_pluginName = plugin->fileName;
		if (!_pluginName.empty()) {
			_pluginName += ' ';
		}
	}

	auto cache = EditorIDCache::GetSingleton();
	_editorID = cache->GetEditorID(a_form);
	if (!_editorID.empty()) {
		_editorID += ' ';
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
	CPrint("%s%s: %s(%08X)%s", _pluginName.c_str(), typeString.data(), _editorID.c_str(), _formID, _fullName.c_str());
}

Help::Setting::Setting(const RE::Setting* a_setting) :
	_setting(a_setting),
	_name(a_setting->GetName())
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
	auto console = RE::ConsoleLog::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->VPrint(a_fmt, args);
		va_end(args);
	}
}

void Help::ParseParams(RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, MatchString& a_matchString, Filter& a_filter, FormType& a_formType)
{
	RE::SCRIPT_FUNCTION::StringChunk* strChunk = nullptr;
	RE::SCRIPT_FUNCTION::IntegerChunk* intChunk = nullptr;

	if (a_scriptData->numParams < 1) {
		return;
	}

	strChunk = a_scriptData->GetStringChunk();
	a_matchString = strChunk->GetString();
	if (a_scriptData->numParams < 2) {
		return;
	}

	intChunk = strChunk->GetNext()->AsInteger();
	a_filter = static_cast<FilterType>(intChunk->GetInteger());
	if (a_scriptData->numParams < 3) {
		return;
	}

	strChunk = intChunk->GetNext()->AsString();
	a_formType = strChunk->GetString();
}

bool Help::Match(const std::string_view& a_haystack)
{
	if (a_haystack.length() == 0 || _needle.length() > a_haystack.length()) {
		return false;
	}

	std::string haystack(a_haystack);
	for (auto& ch : haystack) {
		ch = static_cast<char>(std::tolower(ch));
	}

	const auto [first, last] =
		boost::algorithm::knuth_morris_pratt_search(
			haystack.begin(),
			haystack.end(),
			_needle.begin(),
			_needle.end());
	return first != last;
}

std::string_view Help::GetFullName(RE::TESForm* a_form)
{
	std::string_view result;

	switch (*a_form->formType) {
	case RE::FormType::Dialogue:
		break;
	default:
		{
			auto fullName = a_form->As<RE::TESFullName>();
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
	auto iter = RE::SCRIPT_FUNCTION::GetFirstConsoleCommand();
	EnumerateFunctions(iter, iter + RE::SCRIPT_FUNCTION::Commands::kConsoleCommandsEnd);

	CPrint("----SCRIPT FUNCTIONS--------------------");
	iter = RE::SCRIPT_FUNCTION::GetFirstScriptCommand();
	EnumerateFunctions(iter, iter + RE::SCRIPT_FUNCTION::Commands::kScriptCommandsEnd);
}

void Help::EnumerateFunctions(RE::SCRIPT_FUNCTION* a_beg, RE::SCRIPT_FUNCTION* a_end)
{
	std::string_view commandName;
	std::string_view longNameV;
	std::string_view shortNameV;
	for (auto it = a_beg; it != a_end; ++it) {
		longNameV = safe_string(it->functionName);
		shortNameV = safe_string(it->shortName);
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

				std::string helpText{ safe_string(it->helpString) };
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
		if (Match(setting.first)) {
			settings.emplace(setting.second);
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
		editorID = safe_string(global->GetFormEditorID());
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
	auto [allForms, lock] = RE::TESForm::GetAllForms();
	RE::BSReadLockGuard locker(lock);
	if (!allForms) {
		return std::nullopt;
	}
	auto& formMap = *allForms;

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

		auto cache = EditorIDCache::GetSingleton();
		editorID = cache->GetEditorID(form);
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

	return { std::move(formInfoSet) };
}
