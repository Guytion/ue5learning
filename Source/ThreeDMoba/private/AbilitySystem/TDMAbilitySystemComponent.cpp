// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAbilitySystemComponent.h"

void UTDMAbilitySystemComponent::AbilityActorInfoSet()
{
    // OnGameplayEffectAppliedDelegateToSelf只在服务端调用，因此EffectApplied需要设置客户端RPC。
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(
        this, &UTDMAbilitySystemComponent::ClientEffectApplied);
}

// OnGameplayEffectAppliedDelegateToSelf只在服务端调用，因此EffectApplied需要设置客户端RPC。
void UTDMAbilitySystemComponent::ClientEffectApplied_Implementation(
    UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& EffectSpec,
    FActiveGameplayEffectHandle ActiveEffectHandle
)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    EffectAssetTags.Broadcast(TagContainer);
}

void UTDMAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        /* if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
            AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
            GiveAbility(AbilitySpec);
        } */
        if(AbilitySpec.Ability != nullptr) GiveAbility(AbilitySpec);
        // GiveAbilityAndActivateOnce(AbilitySpec); // 在授予能力后立即激活一次
    }
    bStartupAbilitiesGiven = true; // 授予能力后将标志设为真
    AbilitiesGivenDelegate.Broadcast();
}

void UTDMAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
    {
        // FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        // AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped); // 激活监听事件技能，使角色可以升级
        // GiveAbilityAndActivateOnce(AbilitySpec);
    }
}