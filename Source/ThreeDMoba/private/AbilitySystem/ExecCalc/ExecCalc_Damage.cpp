// Copyright Guytion's Studio


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "TDMGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/TDMAttributeSet.h"

UExecCalc_Damage::UExecCalc_Damage()
{

}

void UExecCalc_Damage::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
    ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

    // 从Source和Target上收集Tags
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    // 硬编码做测试
    float Damage = Spec.GetSetByCallerMagnitude(FTDMGameplayTags::Get().Damage_Physical, false); // 找不到相应的DamageTypeTag，返回0

    const FGameplayModifierEvaluatedData EvaluatedData(UTDMAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}