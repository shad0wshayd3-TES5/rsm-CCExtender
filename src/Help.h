#pragma once

class Help
{
public:
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr);
	static void Register();

private:
	class FormInfo
	{
	public:
		FormInfo() = delete;
		FormInfo(const FormInfo&) = default;
		FormInfo(FormInfo&&) = default;
		FormInfo(RE::TESForm* a_form);

		~FormInfo() = default;

		FormInfo& operator=(const FormInfo&) = default;
		FormInfo& operator=(FormInfo&&) = default;

		bool operator<(const FormInfo& a_rhs) const;

		RE::FormType GetFormType() const;
		void Print() const;

	private:
		std::string _pluginName;
		std::string _editorID;
		std::string _fullName;
		RE::FormID _formID;
		RE::FormType _formType;
	};

	class Setting
	{
	public:
		Setting() = delete;
		Setting(const Setting&) = default;
		Setting(Setting&&) = default;
		Setting(const RE::Setting* a_setting);

		~Setting() = default;

		Setting& operator=(const Setting&) = default;
		Setting& operator=(Setting&&) = default;

		bool operator<(const Setting& a_rhs) const;

		void Print() const;

	private:
		const RE::Setting* _setting;
		std::string _name;
	};

	enum class FilterType : SInt32
	{
		kAll = 0,
		kFunctions = 1,
		kSettings = 2,
		kGlobals = 3,
		kForms = 4
	};

	using MatchString = std::optional<std::string>;
	using Filter = std::optional<FilterType>;
	using FormType = std::optional<std::string>;

	Help() = delete;
	Help(const Help&) = delete;
	Help(Help&&) = delete;
	~Help() = delete;

	Help& operator=(const Help&) = delete;
	Help& operator=(Help&&) = delete;

	static const char* HelpStr();
	static void CPrint(const char* a_fmt, ...);
	static void ParseParams(RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, MatchString& a_matchString, Filter& a_filter, FormType& a_formType);
	static bool Match(const std::string_view& a_haystack);
	static std::string_view GetFullName(RE::TESForm* a_form);
	static void EnumerateFunctions();
	static void EnumerateFunctions(RE::SCRIPT_FUNCTION* a_beg, RE::SCRIPT_FUNCTION* a_end);
	static void EnumerateSettings();
	static void EnumerateGlobals();
	static void EnumerateForms(const FormType& a_formType);
	static std::optional<std::set<FormInfo>> GatherFormInfo(const FormType& a_formType);

	static constexpr char LONG_NAME[] = "Help";
	static constexpr char SHORT_NAME[] = "";
	static inline std::string _needle{};
#if _DEBUG
	static inline StopWatch _stopWatch{};
#endif
};
