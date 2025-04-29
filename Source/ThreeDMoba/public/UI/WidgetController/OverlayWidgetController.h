// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/WidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "覆层控件控制器"))
class THREEDMOBA_API UOverlayWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|属性")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|属性")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|属性")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|属性")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|属性")
	FOnAttributeChangedSignature OnQiChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|玩家状态")
	FOnPlayerStatChangedSignature OnLevelChanged;
};
