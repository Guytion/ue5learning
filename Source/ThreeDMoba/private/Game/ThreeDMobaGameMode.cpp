// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/ThreeDMobaGameMode.h"
#include "Character/ThreeDMobaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThreeDMobaGameMode::AThreeDMobaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
