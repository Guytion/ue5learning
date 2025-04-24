// Copyright Guytion's Studio


#include "Input/InputConfig.h"

const UInputAction* UInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    for (const FTDMInputAction& Action : AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag == InputTag)
        {
            return Action.InputAction;
        }
    }
    if (bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("在输入配置[%s]中找不到对应输入标签[%s]的输入动作"), *GetNameSafe(this), *InputTag.ToString());
    }
    return nullptr;
}