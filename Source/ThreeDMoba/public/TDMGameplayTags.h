// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TDMGameplayTags
 * 
 * 包含原生游戏标签的单例
 */

struct FTDMGameplayTags
{
public:

    static const FTDMGameplayTags& Get() { return GameplayTags; }

    static void InitializeNativeGameplayTags();

private:

    static const FTDMGameplayTags GameplayTags;

};
