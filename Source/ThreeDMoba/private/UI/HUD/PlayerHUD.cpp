// Copyright Guytion's Studio


#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/TDMUserWidget.h"

void APlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("覆层控件类没有初始化，请在蓝图中设置BP_PlayerHUD。"));

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UTDMUserWidget>(Widget);

    OverlayWidget->AddToViewport();
}