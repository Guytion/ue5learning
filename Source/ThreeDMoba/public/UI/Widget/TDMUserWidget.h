// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDMUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "设置控件控制器"))
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "控件控制器"))
	TObjectPtr<UObject> WidgetController;

protected:
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "控件控制器已设置"))
	void WidgetControllerSet();
};
