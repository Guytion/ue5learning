// Copyright Guytion's Studio


#include "AbilitySystem/ExecCalc/ExecCalc_MeleeAttack.h"
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "DamageStatics.h"

float UExecCalc_MeleeAttack::GetDamageValue(
    const FGameplayEffectSpec& Spec,
    FGameplayTag Tag,
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    const FAggregatorEvaluateParameters& EvaluateParams
) const
{
    float DamageValue = Super::GetDamageValue(Spec, Tag, ExecutionParams, EvaluateParams);
    if (DamageValue <= 0.f) return DamageValue;

    float SourceMeleeAttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        DamageStatics().MeleeAttackPowerDef, EvaluateParams, SourceMeleeAttackPower);
    SourceMeleeAttackPower = FMath::Clamp(SourceMeleeAttackPower, 0.f, 160.f);

    return DamageValue * SourceMeleeAttackPower;
}