// Copyright Guytion's Studio


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TDMAttributeSet.h"
#include "Player/TDMPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    Super::BroadcastInitialValues();

    OnHealthChanged.Broadcast(GetHeroAS()->GetHealth());
    OnMaxHealthChanged.Broadcast(GetHeroAS()->GetMaxHealth());
    OnManaChanged.Broadcast(GetHeroAS()->GetMana());
    OnMaxManaChanged.Broadcast(GetHeroAS()->GetMaxMana());
    OnQiChanged.Broadcast(GetHeroAS()->GetQi());

    OnLevelChanged.Broadcast(GetHeroPS()->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnHealthChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnManaChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetMaxManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxManaChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetQiAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnQiChanged.Broadcast(Data.NewValue);
        }
    );

    GetHeroPS()->OnLevelChangedDelegate.AddLambda(
        [this](int32 NewLevel)
        {
            OnLevelChanged.Broadcast(NewLevel);
        }
    );
}