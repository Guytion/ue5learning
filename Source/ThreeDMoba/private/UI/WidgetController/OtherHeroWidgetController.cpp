// Copyright Guytion's Studio


#include "UI/WidgetController/OtherHeroWidgetController.h"

void UOtherHeroWidgetController::BroadcastInitialValues()
{
    Super::BroadcastInitialValues();

    OnHealthChanged.Broadcast(GetHeroAS()->GetHealth());
    OnMaxHealthChanged.Broadcast(GetHeroAS()->GetMaxHealth());
    OnManaChanged.Broadcast(GetHeroAS()->GetMana());
    OnMaxManaChanged.Broadcast(GetHeroAS()->GetMaxMana());
}

void UOtherHeroWidgetController::BindCallbacksToDependencies()
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
            if (Data.NewValue > 0.f)
            {
                OnMaxHealthChanged.Broadcast(Data.NewValue);
            }
            else
            {
                OnMaxHealthChanged.Broadcast(GetHeroAS()->GetMaxHealth());
            }
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
            if (Data.NewValue > 0.f)
            {
                OnMaxManaChanged.Broadcast(Data.NewValue);
            }
            else
            {
                OnMaxManaChanged.Broadcast(GetHeroAS()->GetMaxMana());
            }
        }
    );
}