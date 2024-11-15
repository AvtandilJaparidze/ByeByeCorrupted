#include <iostream>
#include <API/ARK/Ark.h>

#pragma comment(lib, "ArkApi.lib")

DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalStructure*, float, FDamageEvent*, AController*, AActor*);

float Hook_APrimalStructure_TakeDamage(APrimalStructure* _this, float Damage, FDamageEvent* DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (_this->TargetingTeamField() != 0)
	{
		if (EventInstigator)
		{
			ACharacter* character = EventInstigator->CharacterField();

			if (character != nullptr && character->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
			{
				APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(character);

				FString descr;
				dino->GetDinoDescriptiveName(&descr);

				if (descr.Contains("Corrupted"))
				{
					return 0;
				}

			}
		}
	}

	return APrimalStructure_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser);
}

void Load()
{
	Log::Get().Init("ByeByeCorrupted");

	ArkApi::GetHooks().SetHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage,
		&APrimalStructure_TakeDamage_original);
}

void Unload()
{
	ArkApi::GetHooks().DisableHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}