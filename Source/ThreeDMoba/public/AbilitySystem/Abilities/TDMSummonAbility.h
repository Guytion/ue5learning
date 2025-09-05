// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDMGameplayAbility.h"
#include "TDMSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMSummonAbility : public UTDMGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "获取召唤位置"))
	TArray<FVector> GetSpawnLocation();

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "召唤的最小数量"))
	int32 NumMinions = 3; // 召唤的最小数量

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "召唤类"))
	TArray<TSubclassOf<AActor>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "最小生成距离"))
	float MinSpawnDistance = 50.0f; // 最小生成距离

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "最大生成距离"))
	float MaxSpawnDistance = 250.0f; // 最大生成距离

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "散布角度"))
	float SpawnSpread = 60.f;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "获取随机召唤类"))
	TSubclassOf<AActor> GetRandomMinionClass();
};
