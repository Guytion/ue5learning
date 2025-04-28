// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

private:

	UPROPERTY()
	TObjectPtr<UTDMUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "覆层控件类"))
	TSubclassOf<UTDMUserWidget> OverlayWidgetClass;
};
