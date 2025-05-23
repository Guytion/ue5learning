// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_QiAccumulation.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UMMC_QiAccumulation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UMMC_QiAccumulation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition AgilityDef; // 定义属性捕获定义，用于获取Agility属性的值

};
