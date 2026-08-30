#include "SelectedRefColor.h"

#include "Settings.h"

void SelectedRefColor::InstallHooks()
{
	REL::Relocation<std::uintptr_t> vtbl{ RE::VTABLE_Console[0] };  // Console vtbl
	_processMessage = vtbl.write_vfunc(0x4, ProcessMessage);
	REX::INFO("Installed hooks for {}"sv, typeid(SelectedRefColor).name());
}

auto SelectedRefColor::ProcessMessage(RE::IMenu* a_menu, RE::UIMessage& a_message)
	-> UIResult
{
	using MessageType = RE::UI_MESSAGE_TYPE;

	switch (*a_message.type)
	{
	case MessageType::kUpdate:
	case MessageType::kScaleformEvent:
		if (a_menu->OnStack())
		{
			const auto result = _processMessage(a_menu, a_message);
			if (_cachedRef != RE::Console::GetSelectedRef())
			{
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
	auto         refr = _cachedRef;
	RE::NiColorA tint{ 0.0f, 0.0f, 0.0f, 0.0f };

	SKSE::GetTaskInterface()->AddTask(
		[refr, tint]()
		{
			if (refr)
			{
				if (auto root = refr->Get3D())
				{
					root->TintScenegraph(tint);
				}
			}
		});
}

void SelectedRefColor::SetColor()
{
	auto         refr = _cachedRef;
	RE::NiColorA tint = Settings::consoleSelectedRefColor;

	SKSE::GetTaskInterface()->AddTask(
		[refr, tint]()
		{
			if (refr)
			{
				if (auto root = refr->Get3D())
				{
					root->TintScenegraph(tint);
				}
			}
		});
}

void SelectedRefColor::UpdateRef()
{
	_cachedRef = RE::Console::GetSelectedRef();
}
