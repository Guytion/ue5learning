// Copyright Guytion's Studio


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
    if (LevelUpInformation.IsEmpty()) return 1;
    for (int32 Level = 1; Level < LevelUpInformation.Num() + 1; Level++)
    {
        if (XP < LevelUpInformation[Level].LevelUpRequirement)
        {
            return Level;
        }
    }
    return LevelUpInformation.Num() + 1;
}