// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ThreeDMobaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ThreeDMoba/Weapon.h"
#include "HeroCharacter.generated.h"

class UCameraComponent;
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

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (DisplayName = "摄像机摇臂"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (DisplayName = "跟随摄像机"))
	TObjectPtr<UCameraComponent> FollowCamera;

	bool CanSeeActor(const AActor* TargetActor) const;

private:

	int32 LockEnimyIndex;
};
