// Copyright Guytion's Studio


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "TDMGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/TDMAttributeSet.h"

struct TDMDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(MeleeAttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(RangedAttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MagicAttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MissChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);

    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(WaterResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(EarthResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);

    TDMDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, MeleeAttackPower, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, RangedAttackPower, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, MagicAttackPower, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, MissChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, CriticalHitChance, Source, false);

        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, PhysicalResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, FireResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, LightningResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, WaterResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, EarthResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UTDMAttributeSet, ArcaneResistance, Target, false);
    }
};

static const TDMDamageStatics& DamageStatics()
{
    static TDMDamageStatics DStatics;
    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    RelevantAttributesToCapture.Add(DamageStatics().MeleeAttackPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().RangedAttackPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().MagicAttackPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().MissChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);

    RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().WaterResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().EarthResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
    const FTDMGameplayTags& Tags = FTDMGameplayTags::Get();

    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_MeleeAttackPower, DamageStatics().MeleeAttackPowerDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_RangedAttackPower, DamageStatics().RangedAttackPowerDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_MagicAttackPower, DamageStatics().MagicAttackPowerDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_MissChance, DamageStatics().MissChanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);

    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Water, DamageStatics().WaterResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Earth, DamageStatics().EarthResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);

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

    // 从SetByCaller强度中获取伤害
    float Damage = 0.f;
    for (const TTuple<FGameplayTag, FGameplayTag>& Pair : Tags.DamageTypestoResistances)
    {
        const FGameplayTag DamageTypeTag = Pair.Key;
        const FGameplayTag ResistanceTag = Pair.Value;

        checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("ExecCalc_Damage的标签捕获字典没有包含标签： %s"), *ResistanceTag.ToString());
        const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

        float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false); // 找不到相应的DamageTypeTag，返回0
        if (DamageTypeValue <= 0.f)
        {
            continue;
        }

        float Resistance = 0.f;
	    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParams, Resistance);
        Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

        DamageTypeValue *= (100.f - Resistance) / 100.f; // 属性抗性使用减乘

        Damage += DamageTypeValue;
    }
    

    const FGameplayModifierEvaluatedData EvaluatedData(UTDMAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}