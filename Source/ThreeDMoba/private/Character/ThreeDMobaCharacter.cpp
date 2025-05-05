// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ThreeDMobaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "TDMGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "ThreeDMoba/ThreeDMoba.h"
#include "ActorComponent/WeaponMeshComponent.h"

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
    GetCharacterMovement()->MaxSimulationTimeStep = 0.008f; // 提高物理模拟精度
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Linear; // 线性插值模式
    GetCharacterMovement()->NetworkSimulatedSmoothLocationTime = 0.1f; // 平滑时间窗口
	// 提高同步精度
    NetUpdateFrequency = 144.0f; // 默认为100
    MinNetUpdateFrequency = 120.0f;
    
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("Weapon_R"));
	Weapon->SetCollisionObjectType(ECC_Weapon);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
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
	DOREPLIFETIME_CONDITION_NOTIFY(AThreeDMobaCharacter, CombatTarget, COND_None, REPNOTIFY_Always);
	// DOREPLIFETIME_CONDITION_NOTIFY(AThreeDMobaCharacter, WeaponTipSocketName, COND_None, REPNOTIFY_Always);
}

void AThreeDMobaCharacter::LookAtTarget_Implementation(AActor* TargetActor, FRotator& LookAtRotation)
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
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(InitVitalAttributes, 1.f);
}

void AThreeDMobaCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
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

void AThreeDMobaCharacter::Jump()
{
	if (GetLocalRole() == ROLE_AutonomousProxy) {
        ServerJump(); // 显式调用RPC
    }
    Super::Jump();
}

void AThreeDMobaCharacter::ServerJump_Implementation()
{
    if (CanJump()) {
        Super::Jump();
    }
}

void AThreeDMobaCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	if (HasAuthority())
	{
		CombatTarget = InCombatTarget;
	}
	else
	{
		ServerSetCombatTarget(InCombatTarget);
	}
	
}

AActor* AThreeDMobaCharacter::GetCombatTarget_Implementation() const
{
	if (CombatTarget)
	{
		return CombatTarget;
	}
	else
	{
		return nullptr;
	}
}

void AThreeDMobaCharacter::ServerSetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

void AThreeDMobaCharacter::OnRep_CombatTarget()
{
	
}

TArray<FTaggedMontage> AThreeDMobaCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FVector AThreeDMobaCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	const FTDMGameplayTags& GameplayTags = FTDMGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	return FVector();
}

void AThreeDMobaCharacter::SetIsAttacking_Implementation(bool IsAttacking)
{
	if (IsValid(Weapon))
	{
		IsAttacking ? Weapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly) : Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

UWeaponMeshComponent* AThreeDMobaCharacter::GetWeapon_Implementation()
{
	if (UWeaponMeshComponent* WeaponMesh = Cast<UWeaponMeshComponent>(Weapon))
	{
		return WeaponMesh;
	}
	return nullptr;
}

UAnimMontage* AThreeDMobaCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}