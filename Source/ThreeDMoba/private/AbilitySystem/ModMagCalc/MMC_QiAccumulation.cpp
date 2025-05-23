// Copyright Guytion's Studio


#include "AbilitySystem/ModMagCalc/MMC_QiAccumulation.h"
#include "AbilitySystem/TDMAttributeSet.h"

UMMC_QiAccumulation::UMMC_QiAccumulation()
{
    AgilityDef.AttributeToCapture = UTDMAttributeSet::GetAgilityAttribute(); // 设置要捕获的属性为Agility
    AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // 捕获目标的属性值
    AgilityDef.bSnapshot = false; // 不需要快照，实时获取属性值

    RelevantAttributesToCapture.Add(AgilityDef); // 将属性捕获定义添加到相关属性列表中
}

float UMMC_QiAccumulation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    // 从Source和Target上收集Tags
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = SourceTags;
    EvaluateParams.TargetTags = TargetTags;

    const float m = FMath::Loge(0.5f / 0.07f) / FMath::Loge(FMath::Loge(75.f) / FMath::Loge(25.f)); // 指数参数
    const float k = 0.07f / FMath::Pow(FMath::Loge(25.f), m); // 比例参数
    // UE_LOG(LogTemp, Warning, TEXT("m: %f, k: %f"), m, k); // 打印日志(调试用，可删除或注释掉

    // 获取Agility属性的值
    float Agility = 0.0f;
    GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluateParams, Agility);
    Agility = FMath::Clamp(Agility, 0.0f, 75.f);

    return 1 + k * FMath::Pow(FMath::Loge(Agility), m); // 计算并返回最终的属性值
}