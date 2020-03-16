#include "SelectedRefColor.h"

#include <typeinfo>

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/API.h"

#include "Settings.h"


void SelectedRefColor::InstallHooks()
{
	REL::Offset<std::uintptr_t> vtbl = REL::ID(268119);	 // Console vtbl
	_processMessage = vtbl.WriteVFunc(0x4, ProcessMessage);
	_MESSAGE("Installed hooks for %s", typeid(SelectedRefColor).name());
}


auto SelectedRefColor::ProcessMessage(RE::IMenu* a_menu, RE::UIMessage& a_message)
	-> UIResult
{
	using MessageType = RE::UI_MESSAGE_TYPE;

	switch (a_message.type) {
	case MessageType::kUpdate:
	case MessageType::kScaleformEvent:
		if (a_menu->OnStack()) {
			auto result = _processMessage(a_menu, a_message);
			if (_cachedRef != RE::Console::GetSelectedRef()) {
				ClearColor();
				UpdateRef();
				SetColor();
			}
			return result;
		}
		break;
	case MessageType::kShow:
		UpdateRef();
		SetColor();
		break;
	case MessageType::kHide:
		ClearColor();
		break;
	default:
		break;
	}

	return _processMessage(a_menu, a_message);
}


void SelectedRefColor::ClearColor()
{
	auto task = SKSE::GetTaskInterface();
	auto ref = _cachedRef;
	task->AddTask([ref]() {
		auto obj3D = ref ? ref->Get3D() : nullptr;
		if (obj3D) {
			RE::NiColorA color(0.0, 0.0, 0.0, 0.0);
			obj3D->TintScenegraph(color);
		}
	});
}


void SelectedRefColor::SetColor()
{
	auto task = SKSE::GetTaskInterface();
	auto ref = _cachedRef;
	task->AddTask([ref]() {
		auto obj3D = ref ? ref->Get3D() : nullptr;
		if (obj3D) {
			obj3D->TintScenegraph(*Settings::consoleSelectedRefColor);
		}
	});
}


void SelectedRefColor::UpdateRef()
{
	_cachedRef = RE::Console::GetSelectedRef();
}
