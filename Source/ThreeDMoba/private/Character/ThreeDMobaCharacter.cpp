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
#include "Net/UnrealNetwork.h"

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

	// 启用移动组件同步
    GetCharacterMovement()->SetIsReplicated(true);
	// 提高同步精度
    NetUpdateFrequency = 66.0f; // 默认为100
    MinNetUpdateFrequency = 33.0f;
    
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = false; 
}

void AThreeDMobaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AThreeDMobaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AThreeDMobaCharacter, CharacterRotation, COND_None, REPNOTIFY_Always);
}

void AThreeDMobaCharacter::LookAtTarget_Implementation(const AActor* TargetActor, FRotator& LookAtRotation)
{
	if (TargetActor != nullptr)
	{
		FVector Start = GetActorLocation();
		FVector End = TargetActor->GetActorLocation();
		// 计算旋转角度以看向目标Actor
		LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		
		ServerRotateToTarget(LookAtRotation);
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

bool AThreeDMobaCharacter::CanSeeActor_Implementation(const AActor* TargetActor) const
{
	if (!TargetActor)
	{
		return false;
	}
	FHitResult Hit;

	FVector Start = GetActorLocation();
	FVector End = TargetActor->GetActorLocation();
	float Distance = FVector::DistXY(Start, End);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	float DeltaAngleDegrees = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, LookAtRotation.Yaw);
	if (DeltaAngleDegrees < 70.f && DeltaAngleDegrees > -70.f && Distance < 1000.f)
	{
		ECollisionChannel Channel = ECC_Visibility; // 使用Visibility通道进行射线检测
	
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 忽略自身
		QueryParams.AddIgnoredActor(TargetActor); // 忽略目标Actor

		// 执行射线检测
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
		return !Hit.bBlockingHit; // 如果没有被阻挡，则可以看见目标Actor
	}
	else
	{
		return false; // 如果角度超出范围，则无法看见目标Actor
	}
}

void AThreeDMobaCharacter::ServerRotateToTarget_Implementation(const FRotator& Rotation)
{
	CharacterRotation = Rotation;
	SetActorRotation(Rotation);
}

void AThreeDMobaCharacter::OnRep_CharacterRotation()
{
	// 服务器同步旋转信息到客户端
	if (GetNetMode() != NM_DedicatedServer)
    {
        // 使用插值平滑过渡
        const FRotator CurrentRot = GetActorRotation();
        const FRotator TargetRot = CharacterRotation;
        const FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), 8.0f);
        SetActorRotation(NewRot);
    }
}

FOnASCRegistered& AThreeDMobaCharacter::GetOnASCRegisteredDelegate()
{
	return OnASCRegisteredDelegate;
}