// Copyright Guytion's Studio


#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    Super::BroadcastInitialValues();

    OnHealthChanged.Broadcast(GetHeroAS()->GetHealth());
    OnMaxHealthChanged.Broadcast(GetHeroAS()->GetMaxHealth());
    OnManaChanged.Broadcast(GetHeroAS()->GetMana());
    OnMaxManaChanged.Broadcast(GetHeroAS()->GetMaxMana());
    OnQiChanged.Broadcast(GetHeroAS()->GetQi());
    OnQiLayersChanged.Broadcast(GetHeroAS()->GetQiLayers());

    OnLevelChanged.Broadcast(GetHeroPS()->GetPlayerLevel());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

    GetHeroASC()->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnHealthChanged.Broadcast(Data.NewValue);
        }
    );
    GetHeroASC()->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        }
    );
    GetHeroASC()->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnManaChanged.Broadcast(Data.NewValue);
        }
    );
    GetHeroASC()->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetMaxManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxManaChanged.Broadcast(Data.NewValue);
        }
    );
    GetHeroASC()->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetQiAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnQiChanged.Broadcast(Data.NewValue);
        }
    );
    GetHeroASC()->GetGameplayAttributeValueChangeDelegate(GetHeroAS()->GetQiLayersAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnQiLayersChanged.Broadcast(Data.NewValue);
        }
    );

    GetHeroPS()->OnLevelChangedDelegate.AddLambda(
        [this](int32 NewLevel)
        {
            OnLevelChanged.Broadcast(NewLevel);
        }
    );
}