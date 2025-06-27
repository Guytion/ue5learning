// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "TDMGameplayTags.h"
#include "AbilitySystem/Abilities/TDMGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/TDMAbilitySystemLibrary.h"
#include "ThreeDMoba/ThreeDMoba.h"

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
        if (const UTDMGameplayAbility* TDMAbility = Cast<UTDMGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.DynamicAbilityTags.AddTag(TDMAbility->DefaultKeyMouseInputTag);
            AbilitySpec.DynamicAbilityTags.AddTag(TDMAbility->DefaultJoystickInputTag);
            AbilitySpec.DynamicAbilityTags.AddTag(FTDMGameplayTags::Get().Abilities_Status_Unlocked);
        }
        GiveAbility(AbilitySpec);
        // if(AbilitySpec.Ability != nullptr) GiveAbility(AbilitySpec);
    }
    bStartupAbilitiesGiven = true; // 授予能力后将标志设为真
    AbilitiesGivenDelegate.Broadcast();
}

void UTDMAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        AbilitySpec.DynamicAbilityTags.AddTag(FTDMGameplayTags::Get().Abilities_Status_Unlocked); // 激活监听事件技能，使角色可以升级
        GiveAbilityAndActivateOnce(AbilitySpec);
    }
}

void UTDMAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;
    FScopedAbilityListLock ActiveScopeLoc(*this); // 循环迭代中，锁定能力列表，防止在遍历过程中被修改或删除导致的错误。
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (AbilitySpec.IsActive())
            {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
            }
        }
    }
}

void UTDMAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;
    FScopedAbilityListLock ActiveScopeLoc(*this); // 循环迭代中，锁定能力列表，防止在遍历过程中被修改或删除导致的错误。
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void UTDMAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid()) return;
    FScopedAbilityListLock ActiveScopeLoc(*this); // 循环迭代中，锁定能力列表，防止在遍历过程中被修改或删除导致的错误。
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
        {
            AbilitySpecInputReleased(AbilitySpec);
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
        }
    }
}

void UTDMAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
    UAbilityInfo* AbilityInfo = UTDMAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
    for (const FTDMAbilityInfo& Info : AbilityInfo->AbilityInformation)
    {
        if (!Info.AbilityTag.IsValid()) continue;
        if (Level < Info.LevelRequirement) continue;
        if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
        {
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
            FGameplayTag StatusTag = FTDMGameplayTags::Get().Abilities_Status_Eligible;
            AbilitySpec.DynamicAbilityTags.AddTag(StatusTag);
            GiveAbility(AbilitySpec);
            MarkAbilitySpecDirty(AbilitySpec); // 在技能被授予之后标记为脏，以便同步到客户端
            ClientUpdateAbilityStatus(Info.AbilityTag, StatusTag, 1);
        }
    }
}

void UTDMAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this); // 锁定能力列表以确保线程安全。
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogThreeDMoba, Error, TEXT("执行委托失败，%hs"), __FUNCTION__);
        }
    }
}

FGameplayTag UTDMAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UTDMAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
    {
        if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
        {
            return Tag;
        }
    }
    return FGameplayTag();
}

FGameplayTag UTDMAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
    {
        if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
        {
            return StatusTag;
        }
    }
    return FGameplayTag();
}

FGameplayAbilitySpec* UTDMAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopeLoc(*this);
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (Tag.MatchesTag(AbilityTag))
            {
                return &AbilitySpec;
            }
        }
    }
    return nullptr;
}

void UTDMAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(
    const FGameplayTag& AbilityTag,
    const FGameplayTag& StatusTag,
    int32 AbilityLevel
)
{
    AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}