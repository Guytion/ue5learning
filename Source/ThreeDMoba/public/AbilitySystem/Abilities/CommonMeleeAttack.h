// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TDMDamageGameplayAbility.h"
#include "CommonMeleeAttack.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "普通近战攻击"))
class THREEDMOBA_API UCommonMeleeAttack : public UTDMDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "设置武器伤害"))
	void SetWeaponDamage();
};
