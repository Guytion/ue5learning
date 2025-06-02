// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "TDMAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;
/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UTDMAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/* 关键属性 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "关键属性")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "关键属性")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "关键属性")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "关键属性")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Qi, Category = "关键属性")
	FGameplayAttributeData Qi;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, Qi);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxQi, Category = "关键属性")
	FGameplayAttributeData MaxQi;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, MaxQi);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QiLayers, Category = "关键属性")
	FGameplayAttributeData QiLayers;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, QiLayers);

	/* 主要属性 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "主要属性")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "主要属性")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "主要属性")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, Agility);

	/* 次生属性 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_QiAccumulation, Category = "次生属性")
	FGameplayAttributeData QiAccumulation;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, QiAccumulation);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MeleeAttackPower, Category = "次生属性")
	FGameplayAttributeData MeleeAttackPower;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, MeleeAttackPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RangedAttackPower, Category = "次生属性")
	FGameplayAttributeData RangedAttackPower;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, RangedAttackPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicAttackPower, Category = "次生属性")
	FGameplayAttributeData MagicAttackPower;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, MagicAttackPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "次生属性")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, PhysicalResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MissChance, Category = "次生属性")
	FGameplayAttributeData MissChance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, MissChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BasicMagicResistance, Category = "次生属性")
	FGameplayAttributeData BasicMagicResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, BasicMagicResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "次生属性")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "次生属性")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaterResistance, Category = "次生属性")
	FGameplayAttributeData WaterResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, WaterResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EarthResistance, Category = "次生属性")
	FGameplayAttributeData EarthResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, EarthResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "次生属性")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "次生属性")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "次生属性")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "次生属性")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, ManaRegeneration);

	/* 元属性 */
	UPROPERTY(BlueprintReadOnly, Category = "元属性")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "元属性")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UTDMAttributeSet, IncomingXP);

	/* 回调函数 */

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Qi(const FGameplayAttributeData& OldQi) const;

	UFUNCTION()
	void OnRep_MaxQi(const FGameplayAttributeData& OldMaxQi) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;

	UFUNCTION()
	void OnRep_QiAccumulation(const FGameplayAttributeData& OldQiAccumulation) const;

	UFUNCTION()
	void OnRep_MeleeAttackPower(const FGameplayAttributeData& OldMeleeAttackPower) const;

	UFUNCTION()
	void OnRep_RangedAttackPower(const FGameplayAttributeData& OldRangedAttackPower) const;

	UFUNCTION()
	void OnRep_MagicAttackPower(const FGameplayAttributeData& OldMagicAttackPower) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

	UFUNCTION()
	void OnRep_MissChance(const FGameplayAttributeData& OldMissChance) const;

	UFUNCTION()
	void OnRep_BasicMagicResistance(const FGameplayAttributeData& OldBasicMagicResistance) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

	UFUNCTION()
	void OnRep_WaterResistance(const FGameplayAttributeData& OldWaterResistance) const;

	UFUNCTION()
	void OnRep_EarthResistance(const FGameplayAttributeData& OldEarthResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_QiLayers(const FGameplayAttributeData& OldQiLayers) const;

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	bool HandleIncomingDamage(const FEffectProperties& Props);
	void SendXPEvent(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> SourceController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> TargetController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter = nullptr;
};