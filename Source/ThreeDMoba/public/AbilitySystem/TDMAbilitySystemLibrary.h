// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TDMAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* 角色属性默认值 */

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|角色属性默认值", meta = (DisplayName = "初始化默认属性", DefaultToSelf = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|角色属性默认值", meta = (DisplayName = "授予初始技能", DefaultToSelf = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	/* 游戏机制 */

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏机制", meta = (DisplayName = "是否友好"))
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏机制", meta = (DisplayName = "应用伤害效果"))
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, int32 CharacterLevel);

	UFUNCTION(BlueprintPure, Category = "GAS蓝图函数库|游戏机制")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure, Category = "GAS蓝图函数库|游戏机制")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏机制", meta = (DisplayName = "获取半径内存活玩家", DefaultToSelf = "WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius, const FVector& SphereOrigin);

	/* 游戏效果 */

	UFUNCTION(BlueprintPure, Category = "GAS蓝图函数库|游戏效果", meta = (DisplayName = "获取死亡击退"))
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏效果", meta = (DisplayName = "设置死亡击退"))
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏效果", meta = (DisplayName = "设置是否范围伤害"))
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏效果", meta = (DisplayName = "设置范围伤害内径"))
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏效果", meta = (DisplayName = "设置范围伤害外径"))
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|游戏效果", meta = (DisplayName = "设置范围伤害源头"))
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin);

	/* 伤害效果参数 */

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|伤害效果", meta = (DisplayName = "设置死亡击退方向"))
	static void SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|伤害效果", meta = (DisplayName = "设置范围伤害参数"))
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);

	UFUNCTION(BlueprintCallable, Category = "GAS蓝图函数库|伤害效果")
	static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
};
