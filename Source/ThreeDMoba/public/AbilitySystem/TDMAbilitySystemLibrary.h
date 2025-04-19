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
};
