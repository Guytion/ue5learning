// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FTDMLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "升级所需经验"))
	int32 LevelUpRequirement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "技能点奖励"))
	int32 SpellPointsReward = 1; // 技能点奖励
};
/**
 * 
 */
UCLASS()
class THREEDMOBA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "角色升级信息", meta = (DisplayName = "角色升级信息"))
	TArray<FTDMLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(int32 XP);
};
