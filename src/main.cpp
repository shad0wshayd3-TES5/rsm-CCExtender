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
		{
			Settings::LoadSettings();

			Hooks::Install();
			ObScript::Install();

			break;
		}
		default:
			break;
		}
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	SKSE::AllocTrampoline(1 << 4);
	SKSE::GetMessagingInterface()->RegisterListener(MessageCallback);

	return true;
}
