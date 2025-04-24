// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FTDMInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "输入操作"))
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "输入标签"))
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FTDMInputAction> AbilityInputActions;
};
