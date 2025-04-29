// Copyright Guytion's Studio


#include "UI/WidgetController/WidgetController.h"

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