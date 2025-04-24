// Copyright Guytion's Studio


#include "AbilitySystem/Abilities/TDMDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TDMAbilityTypes.h"

/**
 * @brief 对目标Actor施加伤害效果
 * 
 * @param TargetActor 要施加伤害的目标Actor指针
 * 
 * 函数流程：
 * 1. 创建基于技能等级的伤害效果规格
 * 2. 将计算后的伤害数值与伤害类型绑定
 * 3. 将伤害效果应用到目标的技能系统组件
 */
void UTDMDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
    // 创建伤害效果的规格实例，使用当前技能等级（1.f表示默认计算级别）
    FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
    
    // 根据当前技能等级计算缩放后的伤害数值
    const float ScaledDamage = GetDamageAtLevel();
    
    // 将伤害类型与计算后的数值绑定到效果规格
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
    
    // 将准备好的伤害效果应用到目标Actor的技能系统组件
    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *DamageSpecHandle.Data.Get(),
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
    );
}

FDamageEffectParams UTDMDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(
    AActor* TargetActor,
    bool bOverridePitch,
    float PitchOverride
) const
{
    FDamageEffectParams Params;
    Params.WorldContextObject = GetAvatarActorFromActorInfo();
    Params.DamageGameplayEffectClass = DamageEffectClass;
    Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    Params.BaseDamage = GetDamageAtLevel();
    Params.AbilityLevel = GetAbilityLevel();
    Params.DamageType = DamageType;

    if (IsValid(TargetActor))
    {
        FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
        if (bOverridePitch)
        {
            Rotation.Pitch = PitchOverride;
        }
        
    }

    return Params;
}

float UTDMDamageGameplayAbility::GetDamageAtLevel() const
{
    return Damage.GetValueAtLevel(GetAbilityLevel());
}