// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ThreeDMobaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ThreeDMoba/Weapon.h"
#include "Kismet/KismetSystemLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AThreeDMobaCharacter

AThreeDMobaCharacter::AThreeDMobaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// LockCheckedEnemies = TArray<AActor*>();

	PrimaryActorTick.bCanEverTick = false; 
}

void AThreeDMobaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AThreeDMobaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* if (bIsLocked)
	{
		if (LockCheckedEnemies.Num() > 0)
		{
			AActor* LockedEnemy = LockCheckedEnemies[LockEnimyIndex];
			if (CanSeeActor(LockedEnemy))
			{
				LookAtTarget(LockedEnemy);
			}
			else
			{
				bIsLocked = false;
				LockCheckedEnemies.Empty();
			}
		}
		else
		{
			bIsLocked = false;
			LockCheckedEnemies.Empty();
		}
	} */
	
}

void AThreeDMobaCharacter::LookAtTarget(const AActor* TargetActor)
{
	if (TargetActor != nullptr)
	{
		FVector Start = GetActorLocation();
		FVector End = TargetActor->GetActorLocation();
		// 计算旋转角度以看向目标Actor
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		SetActorRotation(LookAtRotation);
		AddControllerYawInput(FMath::FindDeltaAngleDegrees(GetControlRotation().Yaw, LookAtRotation.Yaw) / 100.f);

	}
}

float AThreeDMobaCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 计算实际伤害
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	// 播放受击动画
	if (GetHitMontage)
	{
		PlayAnimMontage(GetHitMontage);
	}

	return ActualDamage;
}

void AThreeDMobaCharacter::InitAbilityActorInfo()
{

}

void AThreeDMobaCharacter::InitializeDefaultAttributes() const
{
	
}

UAbilitySystemComponent* AThreeDMobaCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}