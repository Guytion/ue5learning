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

};
