// Copyright Guytion's Studio


#include "ActorComponent/TDMWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/HeroCharacter.h"
#include "UI/Widget/TDMUserWidget.h"
#include "UI/WidgetController/OtherHeroWidgetController.h"

void UTDMWidgetComponent::InitWidget()
{
    Super::InitWidget();

    if (GetOwner<AHeroCharacter>()->IsLocallyControlled()) return;

    UTDMUserWidget* OtherHeroWidget = Cast<UTDMUserWidget>(GetWidget());
    const FWidgetControllerParams OtherHeroParams = GetWidgetControllerParams();
    if (OtherHeroWidget and OtherHeroParams.AttributeSet)
    {
        UOtherHeroWidgetController* WidgetController = GetOtherHeroWidgetController(OtherHeroParams);
        OtherHeroWidget->SetWidgetController(WidgetController);
        WidgetController->BroadcastInitialValues();
    }
}

UOtherHeroWidgetController* UTDMWidgetComponent::GetOtherHeroWidgetController(const FWidgetControllerParams& WCParams)
{
    checkf(OtherHeroWidgetControllerClass, TEXT("其他英雄控件控制器类没有初始化，请设置英雄角色蓝图。"));

    if (OtherHeroWidgetController == nullptr)
    {
        OtherHeroWidgetController = NewObject<UOtherHeroWidgetController>(this, OtherHeroWidgetControllerClass);
        OtherHeroWidgetController->SetWidgetControllerParams(WCParams);
        OtherHeroWidgetController->BindCallbacksToDependencies();
    }
    return OtherHeroWidgetController;
}

FWidgetControllerParams UTDMWidgetComponent::GetWidgetControllerParams()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    ATDMPlayerState* OtherHeroPS = GetOwner<AHeroCharacter>()->GetPlayerState<ATDMPlayerState>();
    UAbilitySystemComponent* OtherHeroASC = GetOwner<AHeroCharacter>()->GetAbilitySystemComponent();
    UAttributeSet* OtherHeroAS = GetOwner<AHeroCharacter>()->GetAttributeSet();
    return FWidgetControllerParams(PC, OtherHeroPS, OtherHeroASC, OtherHeroAS);
}