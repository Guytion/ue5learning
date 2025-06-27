// Copyright Guytion's Studio


#include "AbilitySystem/Data/AbilityInfo.h"
#include "ThreeDMoba/ThreeDmoba.h"

FTDMAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
    for (const FTDMAbilityInfo& Info : AbilityInformation)
    {
        if (Info.AbilityTag == AbilityTag)
        {
            return Info;
        }
    }
    if (bLogNotFound)
    {
        UE_LOG(LogThreeDMoba, Error, TEXT("没有找到对应的Ability信息，AbilityTag: %s"), *AbilityTag.ToString());
    }
    
    return FTDMAbilityInfo();
}