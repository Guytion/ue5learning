// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Player/TDMPlayerController.h"
#include "Player/TDMPlayerState.h"
#include "AbilitySystem/TDMAttributeSet.h"
#include "WidgetController.generated.h"

class UTDMAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, StatValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);
/**
 * 
 */
UCLASS(meta = (DisplayName = "控件控制器"))
class THREEDMOBA_API UWidgetController : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ATDMPlayerController> TDMPlayerController;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ATDMPlayerState> TDMPlayerState;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTDMAbilitySystemComponent> TDMAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTDMAttributeSet> TDMAttributeSet;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "控件数据")
	// TObjectPtr<UAbilityInfo> AbilityInfo;

	ATDMPlayerController* GetHeroPC();
	ATDMPlayerState* GetHeroPS();
	UTDMAbilitySystemComponent* GetHeroASC();
	UTDMAttributeSet* GetHeroAS();

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();

	// UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	// FAbilityInfoSignature AbilityInfoDelegate;

	// 广播技能信息
	// void BroadcastAbilityInfo();
};
