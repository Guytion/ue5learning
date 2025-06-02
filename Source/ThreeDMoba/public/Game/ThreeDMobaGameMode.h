// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScalableFloat.h"
#include "ThreeDMobaGameMode.generated.h"

UCLASS(minimalapi)
class AThreeDMobaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThreeDMobaGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "基础数据", meta = (DisplayName = "经验值奖励"))
	FScalableFloat XPReward = FScalableFloat();
};



