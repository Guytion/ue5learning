// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "ExecCalc_MagicAttack.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UExecCalc_MagicAttack : public UExecCalc_Damage
{
	GENERATED_BODY()
	
protected:

	virtual float GetDamageValue(
		const FGameplayEffectSpec& Spec,
		FGameplayTag Tag,
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		const FAggregatorEvaluateParameters& EvaluateParams
	) const override;
};
