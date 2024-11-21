#pragma once

class SelectedRefColor
{
public:
	static void InstallHooks();

private:
	using UIResult = RE::UI_MESSAGE_RESULTS;

	SelectedRefColor() = delete;
	SelectedRefColor(const SelectedRefColor&) = delete;
	SelectedRefColor(SelectedRefColor&&) = delete;
	virtual ~SelectedRefColor() = delete;

	SelectedRefColor& operator=(const SelectedRefColor&) = delete;
	SelectedRefColor& operator=(SelectedRefColor&&) = delete;

	static UIResult ProcessMessage(RE::IMenu* a_menu, RE::UIMessage& a_message);

	static void ClearColor();
	static void SetColor();
	static void UpdateRef();

	inline static REL::Relocation<decltype(&RE::IMenu::ProcessMessage)> _processMessage{};
	inline static RE::TESObjectREFRPtr _cachedRef{};
};
