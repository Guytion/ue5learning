// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDMDamageGameplayAbility.h"
#include "TDMFireBlast.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "火焰爆发"))
class THREEDMOBA_API UTDMFireBlast : public UTDMDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "火球数量"))
	int32 NumFireBalls = 12;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "生成火球"))
	TArray<AFireBall*> SpawnFireBalls();

private:

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "火球类"))
	TSubclassOf<AFireBall> FireBallClass;
};
