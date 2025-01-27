// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThreeDMobaGameMode.h"
#include "ThreeDMobaCharacter.h"
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
