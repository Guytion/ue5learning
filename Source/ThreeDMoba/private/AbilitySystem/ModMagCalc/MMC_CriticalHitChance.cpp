// Copyright Guytion's Studio


#include "AbilitySystem/ModMagCalc/MMC_CriticalHitChance.h"
#include "AbilitySystem/TDMAttributeSet.h"

UMMC_CriticalHitChance::UMMC_CriticalHitChance()
{
    AgilityDef.AttributeToCapture = UTDMAttributeSet::GetAgilityAttribute(); // 设置要捕获的属性为Agility
    AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // 捕获目标的属性值
    AgilityDef.bSnapshot = false; // 不需要快照，实时获取属性值

    RelevantAttributesToCapture.Add(AgilityDef); // 将属性捕获定义添加到相关属性列表中
}

float UMMC_CriticalHitChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    // 从Source和Target上收集Tags
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    // 获取Agility属性的值
    float Agility = 0.0f;
    GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluateParams, Agility);
    Agility = FMath::Clamp(Agility, 0.0f, 75.f);

    return (13 * FMath::Square(Agility) + 32 * Agility - 45) / 888;
}