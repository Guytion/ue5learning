// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ThreeDMobaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ThreeDMoba/Weapon.h"
#include "Interaction/PlayerInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/WidgetComponent.h"
#include "HeroCharacter.generated.h"

class UCameraComponent;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class THREEDMOBA_API AHeroCharacter : public AThreeDMobaCharacter,
public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	AHeroCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bIsEquipped = false;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	AWeapon* GetAttachedWeapon() const;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	/* Player Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetLockRotation_Implementation(bool bIsLocked) override;
	/* Player Interface 结束 */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxManaChanged;

	void BroadcastInitialValues();
	void BindCallbacksToDependencies();

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (DisplayName = "摄像机摇臂"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (DisplayName = "跟随摄像机"))
	TObjectPtr<UCameraComponent> FollowCamera;

	virtual void BeginPlay() override;

	void AddCharacterAbilities(); // 添加角色能力

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (DisplayName = "血条蓝条显示"))
	TObjectPtr<UWidgetComponent> HeroStatusBar;

private:

	UPROPERTY(EditAnywhere, Category = "英雄|技能", meta = (DisplayName = "起始主动技能组"))
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "英雄|技能", meta = (DisplayName = "起始被动技能组"))
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities; // 起始被动技能为监听经验值

	bool bHighlighted = false; // 用于记录当前是否高亮显示
};
