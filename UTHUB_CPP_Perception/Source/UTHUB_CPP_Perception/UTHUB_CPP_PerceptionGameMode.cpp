// Copyright Epic Games, Inc. All Rights Reserved.

#include "UTHUB_CPP_PerceptionGameMode.h"
#include "UTHUB_CPP_PerceptionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUTHUB_CPP_PerceptionGameMode::AUTHUB_CPP_PerceptionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
