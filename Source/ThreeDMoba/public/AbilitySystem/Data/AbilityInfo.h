// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FTDMAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "法术图标"))
	TObjectPtr<const UTexture2D> Icon = nullptr; // Texture2D是2D纹理

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "背景球体"))
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "等级要求"))
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "技能"))
	TSubclassOf<UGameplayAbility> Ability;
};

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "技能信息")
	TArray<FTDMAbilityInfo> AbilityInformation;

	FTDMAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
