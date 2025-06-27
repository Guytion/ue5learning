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

    if (GetHeroASC())
    {
        // GetHeroASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
        if (GetHeroASC()->bStartupAbilitiesGiven)
        {
            BroadcastAbilityInfo();
        }
        else
        {
            GetHeroASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
        }

        /* GetHeroASC()->EffectAssetTags.AddLambda(
            [this](const FGameplayTagContainer& AssetTags)
            {
                // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString("Pickup"));
                for (const FGameplayTag& Tag : AssetTags)
                {
                    // 举个例子，比如 Tag = Message.HealthPotion
                    // "Message.HealthPotion".MatchesTag("Message") 会返回true，"Message".MatchesTag("Message.HealthPotion") 会返回false
                    FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                    if (Tag.MatchesTag(MessageTag))
                    {
                        const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag); // 这里会触发DataTable的加载，从而触发事件绑定
                        MessageWidgetRowDelegate.Broadcast(*Row);
                    }
                }
            }
        ); */
    }

    GetHeroPS()->OnLevelChangedDelegate.AddLambda(
        [this](int32 NewLevel)
        {
            OnLevelChanged.Broadcast(NewLevel);
        }
    );
}