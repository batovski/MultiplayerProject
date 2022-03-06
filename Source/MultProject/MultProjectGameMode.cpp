// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultProjectGameMode.h"
#include "MultProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultProjectGameMode::AMultProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
