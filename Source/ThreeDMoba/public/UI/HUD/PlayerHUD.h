// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/WidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "PlayerHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UTDMUserWidget;

/**
 * 
 */
UCLASS()
class THREEDMOBA_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

private:

	UPROPERTY()
	TObjectPtr<UTDMUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "覆层控件类"))
	TSubclassOf<UTDMUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "覆层控件控制器类"))
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
