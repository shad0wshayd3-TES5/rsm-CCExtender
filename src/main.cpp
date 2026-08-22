#include "Hooks/Hooks.h"
#include "ObScript/ObScript.h"
#include "Settings.h"

namespace
{
	void MessageCallback(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case SKSE::MessagingInterface::kPostLoad:
			Settings::Load();
			Hooks::Install();
			ObScript::Install();
			break;
		default:
			break;
		}
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse, { .trampoline = true, .trampolineSize = 64 });
	SKSE::GetMessagingInterface()->RegisterListener(MessageCallback);
	return true;
}
