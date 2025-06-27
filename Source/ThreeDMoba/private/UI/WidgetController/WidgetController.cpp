// Copyright Guytion's Studio


#include "UI/WidgetController/WidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
    PlayerController = WCParams.PlayerController;
    PlayerState = WCParams.PlayerState;
    AbilitySystemComponent = WCParams.AbilitySystemComponent;
    AttributeSet = WCParams.AttributeSet;
}

ATDMPlayerController* UWidgetController::GetHeroPC()
{
    if (TDMPlayerController == nullptr)
    {
        TDMPlayerController = Cast<ATDMPlayerController>(PlayerController);
    }
    return TDMPlayerController;
}

ATDMPlayerState* UWidgetController::GetHeroPS()
{
    if (TDMPlayerState == nullptr)
    {
        TDMPlayerState = Cast<ATDMPlayerState>(PlayerState);
    }
    return TDMPlayerState;
}

UTDMAbilitySystemComponent* UWidgetController::GetHeroASC()
{
    if (TDMAbilitySystemComponent == nullptr)
    {
        TDMAbilitySystemComponent = Cast<UTDMAbilitySystemComponent>(AbilitySystemComponent);
    }
    return TDMAbilitySystemComponent;
}

UTDMAttributeSet* UWidgetController::GetHeroAS()
{
    if (TDMAttributeSet == nullptr)
    {
        TDMAttributeSet = Cast<UTDMAttributeSet>(AttributeSet);
    }
    return TDMAttributeSet;
}

void UWidgetController::BroadcastInitialValues()
{
    
}

void UWidgetController::BindCallbacksToDependencies()
{

}

void UWidgetController::BroadcastAbilityInfo()
{
    // 收集所有已被授予的技能，查看它们的信息，然后广播至用户控件。
    if (!GetHeroASC()->bStartupAbilitiesGiven) return;

    FForEachAbility BroadcastDelegate;
    BroadcastDelegate.BindLambda(
        [this](const FGameplayAbilitySpec& AbilitySpec)
        {
            // 寻找对应AbilitySpec的技能标签
            FTDMAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetHeroASC()->GetAbilityTagFromSpec(AbilitySpec));
            Info.InputTag = GetHeroASC()->GetInputTagFromSpec(AbilitySpec);
            Info.StatusTag = GetHeroASC()->GetStatusFromSpec(AbilitySpec);
            AbilityInfoDelegate.Broadcast(Info);
        }
    );
    GetHeroASC()->ForEachAbility(BroadcastDelegate);
}