#pragma once

class FormStringTable
{
public:
	static FormStringTable* GetSingleton();

	std::string_view MapFormTypeToString(RE::FormType a_formType);
	RE::FormType MapStringToFormType(const std::string_view& a_string);

private:
	FormStringTable();
	FormStringTable(const FormStringTable&) = delete;
	FormStringTable(FormStringTable&&) = delete;
	~FormStringTable() = default;

	FormStringTable& operator=(const FormStringTable&) = delete;
	FormStringTable& operator=(FormStringTable&&) = delete;

	void Insert(RE::FormType a_formType, const char* a_string);

	std::unordered_map<RE::FormType, std::string_view> _formTypeToStringMap;
	std::unordered_map<std::string_view, RE::FormType> _stringToFormTypeMap;
};
