// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/WidgetController.h"
#include "OtherHeroWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class THREEDMOBA_API UOtherHeroWidgetController : public UWidgetController
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
};
