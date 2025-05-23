// Copyright Guytion's Studio


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/TDMAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
    StrengthDef.AttributeToCapture = UTDMAttributeSet::GetStrengthAttribute(); // 设置要捕获的属性为Strength
    StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // 捕获目标的属性值
    StrengthDef.bSnapshot = false; // 不需要快照，实时获取属性值

    RelevantAttributesToCapture.Add(StrengthDef); // 将属性捕获定义添加到相关属性列表中
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    // 从Source和Target上收集Tags
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    // 获取Strength属性的值
    float Strength = 0.0f;
    GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluateParams, Strength);
    Strength = FMath::Clamp(Strength, 0.0f, 75.f);

    return 150 * FMath::Loge(Strength) + 3 * Strength + 97;
}