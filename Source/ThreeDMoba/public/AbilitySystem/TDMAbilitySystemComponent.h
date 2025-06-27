// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDMAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void UpdateAbilityStatuses(int32 Level);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	// static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	// static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	// FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	// bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	// static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	// static void ClearSlot(FGameplayAbilitySpec* Spec);

	// static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);

	bool bStartupAbilitiesGiven = false;

protected:

	UFUNCTION(Client, Reliable) // OnGameplayEffectAppliedDelegateToSelf只在服务端调用，因此EffectApplied需要设置客户端RPC。
	void ClientEffectApplied(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle
	);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(
		const FGameplayTag& AbilityTag,
		const FGameplayTag& StatusTag,
		int32 AbilityLevel
	);

	virtual void OnRep_ActivateAbilities() override;
};
