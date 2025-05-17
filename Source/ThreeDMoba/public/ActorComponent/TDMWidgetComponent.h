// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UI/WidgetController/WidgetController.h"
#include "TDMWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	virtual void InitWidget() override;

	UFUNCTION()
	UOtherHeroWidgetController* GetOtherHeroWidgetController(const FWidgetControllerParams& WCParams);

private:

	UPROPERTY()
	TObjectPtr<UOtherHeroWidgetController> OtherHeroWidgetController;

	UPROPERTY(EditAnywhere, Category=UserInterface, meta = (DisplayName = "其他英雄控件控制器类"))
	TSubclassOf<UOtherHeroWidgetController> OtherHeroWidgetControllerClass;

	UFUNCTION()
	FWidgetControllerParams GetWidgetControllerParams();
};
