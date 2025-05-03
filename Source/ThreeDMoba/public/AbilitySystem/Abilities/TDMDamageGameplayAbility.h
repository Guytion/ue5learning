// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDMGameplayAbility.h"
#include "TDMDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "伤害技能"))
class THREEDMOBA_API UTDMDamageGameplayAbility : public UTDMGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "造成伤害"))
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "从类默认值中计算伤害效果参数"))
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		// FVector InRadialDamageOrigin = FVector::ZeroVector,
		// bool bOverrideKnockbackDirection = false,
		// FVector KnockbackDirectionOverride = FVector::ZeroVector,
		// bool bOverrideDeathImpulse = false,
		// FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f
	) const;

	UFUNCTION(BlueprintPure, Category="伤害", meta = (DisplayName = "根据等级计算伤害"))
	virtual float GetDamageAtLevel() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "伤害效果类"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="伤害", meta = (DisplayName = "伤害类型"))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="伤害", meta = (DisplayName = "伤害数值"))
	FScalableFloat Damage;
};
