// Copyright Guytion's Studio


#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/TDMUserWidget.h"

void APlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("覆层控件类没有初始化，请设置BP_PlayerHUD蓝图。"));
    checkf(OverlayWidgetControllerClass, TEXT("覆层控件控制器类没有初始化，请设置BP_PlayerHUD蓝图。"));

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UTDMUserWidget>(Widget);

    const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
    UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
    OverlayWidget->SetWidgetController(WidgetController);
    WidgetController->BroadcastInitialValues();
    OverlayWidget->AddToViewport();
}

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
    if (OverlayWidgetController == nullptr)
    {
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParams(WCParams);
        OverlayWidgetController->BindCallbacksToDependencies();
    }
    return OverlayWidgetController;
}