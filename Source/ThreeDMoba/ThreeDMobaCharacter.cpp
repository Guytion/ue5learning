// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThreeDMobaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	LockCheckedEnemies = TArray<AActor*>();

	PrimaryActorTick.bCanEverTick = true; 
}

void AThreeDMobaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThreeDMobaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThreeDMobaCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThreeDMobaCharacter::Look);

		EnhancedInputComponent->BindAction(SwitchEquipmentAction, ETriggerEvent::Triggered, this, &AThreeDMobaCharacter::SwitchEquipment);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AThreeDMobaCharacter::Attack);

		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &AThreeDMobaCharacter::Lock);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThreeDMobaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AThreeDMobaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		if (!bIsLocked) // 如果没有锁定，才允许旋转
		{
			AddControllerYawInput(LookAxisVector.X);
		}
		else
		{
			int32 EnemiesCount = LockCheckedEnemies.Num();
			if (EnemiesCount > 0)
			{
				if (LookAxisVector.X > 0.8f)
				{
					LockEnimyIndex = (LockEnimyIndex + 1) % EnemiesCount; // 循环选择敌人
				}
				else if(LookAxisVector.X < -0.8f)
				{
					LockEnimyIndex = (LockEnimyIndex - 1 + EnemiesCount) % EnemiesCount; // 循环选择敌人
				}
				FVector Loc = GetActorLocation();
				LockCheckedEnemies.Sort(
					[Loc](const AActor& A, const AActor& B)
					{
						float DistanceA = FVector::DistXY(Loc, A.GetActorLocation());
						float DistanceB = FVector::DistXY(Loc, B.GetActorLocation());
						return DistanceA < DistanceB;
					}
				);
			}
		}
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThreeDMobaCharacter::SwitchEquipment()
{
	bIsEquipped = !bIsEquipped; // 将装备状态切换为相反状态
}

void AThreeDMobaCharacter::Attack()
{
	if (!bIsEquipped)
	{
		return;
	}
}

void AThreeDMobaCharacter::Lock()
{
	bIsLocked = !bIsLocked; // 切换锁定状态

	if (bIsLocked)
	{
		TArray<AActor* > OutActors = TArray<AActor*>();
		LockCheckedEnemies.Empty();

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AThreeDMobaCharacter::StaticClass(), OutActors); // 获取所有ThreeDMobaCharacter类的Actor
		FVector Loc = GetActorLocation();
		for (AActor* Actor : OutActors)
		{
			if (this != Actor && CanSeeActor(Actor))
			{
				LockCheckedEnemies.Add(Actor);
			}
		}
		LockCheckedEnemies.Sort(
			[Loc](const AActor& A, const AActor& B)
			{
				float DistanceA = FVector::DistXY(Loc, A.GetActorLocation());
				float DistanceB = FVector::DistXY(Loc, B.GetActorLocation());
				return DistanceA < DistanceB;
			}
		);
		LockEnimyIndex = 0; // 初始化锁定敌人索引为0
	}
}

bool AThreeDMobaCharacter::CanSeeActor(const AActor* TargetActor) const
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
	float DeltaAngleDegrees = FMath::FindDeltaAngleDegrees(FollowCamera->GetComponentRotation().Yaw, LookAtRotation.Yaw);
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

void AThreeDMobaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLocked)
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
	}
	
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