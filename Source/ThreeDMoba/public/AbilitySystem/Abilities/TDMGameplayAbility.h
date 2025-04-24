// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDMGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (DisplayName = "默认键鼠输入标签"))
	FGameplayTag DefaultKeyMouseInputTag;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (DisplayName = "默认手柄输入标签"))
	FGameplayTag DefaultJoystickInputTag;
};
