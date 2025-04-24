// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "TDMPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInputConfig;
/**
 * 
 */
UCLASS()
class THREEDMOBA_API ATDMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ATDMPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "Variable")
	bool bIsLocked = false;

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SwitchEquipmentAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SwitchRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> SwitchLeft;

	/** 锁定敌人 **/
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;

private:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump();
	void StopJumping();

	/** 装备/卸载装备 **/
	void SwitchEquipment();	

	/** 锁定敌人 **/
	void Lock();
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);
	int32 CurrentLockIndex = INDEX_NONE;
	void FindPotentialTargets();
	void SwitchTarget(int32 Direction);
	void OnSwitchTargetRight();
    void OnSwitchTargetLeft();
	void CancelLock();

	UPROPERTY()
	TArray<AActor*> LockCheckedEnemies;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = (DisplayName = "锁定半径"))
    float LockOnRadius = 1000.0f;

	// 视角控制
    virtual void UpdateRotation(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<UTDMAbilitySystemComponent> TDMAbilitySystemComponent;

	UTDMAbilitySystemComponent* GetASC();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputConfig> InputConfig;
};
