// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ThreeDMobaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ThreeDMoba/Weapon.h"
#include "HeroCharacter.generated.h"

class UCameraComponent;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class THREEDMOBA_API AHeroCharacter : public AThreeDMobaCharacter
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

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (DisplayName = "摄像机摇臂"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (DisplayName = "跟随摄像机"))
	TObjectPtr<UCameraComponent> FollowCamera;

	bool CanSeeActor(const AActor* TargetActor) const;

	void AddCharacterAbilities(); // 添加角色能力

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override; // 初始化默认属性
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

private:

	int32 LockEnimyIndex;

	UPROPERTY(EditAnywhere, Category = "英雄|技能", meta = (DisplayName = "起始主动技能组"))
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "英雄|技能", meta = (DisplayName = "起始被动技能组"))
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities; // 起始被动技能为监听经验值
};
