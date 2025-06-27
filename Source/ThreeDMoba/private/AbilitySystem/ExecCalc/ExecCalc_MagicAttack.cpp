// Copyright Guytion's Studio


#include "AbilitySystem/ExecCalc/ExecCalc_MagicAttack.h"
#include "DamageStatics.h"

float UExecCalc_MagicAttack::GetDamageValue(
    const FGameplayEffectSpec& Spec,
    FGameplayTag Tag,
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    const FAggregatorEvaluateParameters& EvaluateParams
) const
{
    float DamageValue = Super::GetDamageValue(Spec, Tag, ExecutionParams, EvaluateParams);
    if (DamageValue <= 0.f) return DamageValue;

    float SourceMagicAttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        DamageStatics().MagicAttackPowerDef, EvaluateParams, SourceMagicAttackPower);
    SourceMagicAttackPower = FMath::Clamp(SourceMagicAttackPower, 0.f, 210.f);

    return DamageValue * SourceMagicAttackPower;
}