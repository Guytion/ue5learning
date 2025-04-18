// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDMPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "ThreeDMoba/ThreeDMoba.h"

ATDMPlayerController::ATDMPlayerController()
{
    bReplicates = true;
}

void ATDMPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (DefaultMappingContext)
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ATDMPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATDMPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATDMPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDMPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATDMPlayerController::Look);

		EnhancedInputComponent->BindAction(SwitchEquipmentAction, ETriggerEvent::Triggered, this, &ATDMPlayerController::SwitchEquipment);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATDMPlayerController::Attack);

		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &ATDMPlayerController::Lock);
	}
	else
	{
		UE_LOG(LogThreeDMoba, Error, TEXT("'%s' 获取增强输入组件失败，请检查设置！"), *GetNameSafe(this));
	}
}

void ATDMPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    // get right vector 
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // 控制角色移动
    APawn* ControlledPawn = GetPawn<APawn>();
    if (ControlledPawn)
    {
        ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ATDMPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

    APawn* ControlledPawn = GetPawn<APawn>();
	if (!bIsLocked && ControlledPawn) // 如果没有锁定，才允许旋转
    {
        ControlledPawn->AddControllerYawInput(LookAxisVector.X);
    }
    else
    {
        /* int32 EnemiesCount = LockCheckedEnemies.Num();
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
        } */
    }
    ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
}

void ATDMPlayerController::SwitchEquipment()
{
	
}

void ATDMPlayerController::Attack()
{
	
}

void ATDMPlayerController::Lock()
{
	/* bIsLocked = !bIsLocked; // 切换锁定状态

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
	} */
}

void ATDMPlayerController::Jump()
{
    GetCharacter()->Jump(); // 调用角色的跳跃函数
}

void ATDMPlayerController::StopJumping()
{
    GetCharacter()->StopJumping(); // 调用角色的停止跳跃函数
}