// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDMPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "ThreeDMoba/ThreeDMoba.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Input/TDMInputComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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

    if (UTDMInputComponent* TDMInputComponent = Cast<UTDMInputComponent>(InputComponent)) // 需要在项目设置->引擎->输入->默认类->默认输入组件类中设置TDMInputComponent
    {
		// Jumping
		TDMInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATDMPlayerController::Jump);
		TDMInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATDMPlayerController::StopJumping);

		// Moving
		TDMInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATDMPlayerController::Move);

		// Looking
		TDMInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATDMPlayerController::Look);

		TDMInputComponent->BindAction(SwitchEquipmentAction, ETriggerEvent::Triggered, this, &ATDMPlayerController::SwitchEquipment);

		TDMInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &ATDMPlayerController::Lock);
		TDMInputComponent->BindAction(SwitchRight, ETriggerEvent::Completed, this, &ATDMPlayerController::OnSwitchTargetRight);
		TDMInputComponent->BindAction(SwitchLeft, ETriggerEvent::Completed, this, &ATDMPlayerController::OnSwitchTargetLeft);

        TDMInputComponent->BindAbilityActions(InputConfig, this, &ATDMPlayerController::AbilityInputTagPressed, &ATDMPlayerController::AbilityInputTagReleased, &ATDMPlayerController::AbilityInputTagHeld);
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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!bIsLocked) AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ATDMPlayerController::SwitchEquipment()
{
	
}

void ATDMPlayerController::Lock()
{
	bIsLocked = !bIsLocked;

    if (bIsLocked)
    {
        if (GetPawn()->Implements<UPlayerInterface>())
        {
            IPlayerInterface::Execute_SetLockRotation(GetPawn(), bIsLocked);
        }
        else
        {
            return;
        }
        
		FindPotentialTargets();
        if(LockCheckedEnemies.Num() > 0)
        {
            CurrentLockIndex = 0;
            ThisActor = LockCheckedEnemies[CurrentLockIndex];
            HighlightActor(ThisActor);
            ICombatInterface::Execute_SetCombatTarget(GetPawn(), ThisActor);
        }
	}
	else
	{
        CancelLock();
	}
	
}

void ATDMPlayerController::Jump()
{
    GetCharacter()->Jump(); // 调用角色的跳跃函数
}

void ATDMPlayerController::StopJumping()
{
    GetCharacter()->StopJumping(); // 调用角色的停止跳跃函数
}

UTDMAbilitySystemComponent* ATDMPlayerController::GetASC()
{
    if (TDMAbilitySystemComponent == nullptr)
    {
        TDMAbilitySystemComponent = Cast<UTDMAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
    }
    
    return TDMAbilitySystemComponent; // 返回能力系统组件指针
}

void ATDMPlayerController::HighlightActor(AActor* InActor)
{
    if (IsValid(InActor) and InActor->Implements<UPlayerInterface>())
    {
        IPlayerInterface::Execute_HighlightActor(InActor);
    }
}

void ATDMPlayerController::UnHighlightActor(AActor* InActor)
{
    if (IsValid(InActor) and InActor->Implements<UPlayerInterface>())
    {
        IPlayerInterface::Execute_UnHighlightActor(InActor);
    }
}

void ATDMPlayerController::FindPotentialTargets()
{
	LockCheckedEnemies.Empty(); // 清空锁定敌人列表

	// 获取玩家位置
    APawn* ControlledPawn = GetPawn<APawn>();
    if(!ControlledPawn) return;

    TArray<AActor*> OverlapActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // 检测Pawn类型
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(ControlledPawn);

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        ControlledPawn->GetActorLocation(),
        LockOnRadius,
        ObjectTypes,
        ACharacter::StaticClass(), // 替换为你的敌人基类
        IgnoreActors,
        OverlapActors
    );

    for (AActor* OverlapActor : OverlapActors)
    {
        if (ControlledPawn->Implements<UCombatInterface>() and
            ICombatInterface::Execute_CanSeeActor(ControlledPawn, OverlapActor) and
            OverlapActor->Implements<UPlayerInterface>())
        {
            LockCheckedEnemies.Add(OverlapActor);
        }
        
    }

    // 按距离排序
    LockCheckedEnemies.Sort([ControlledPawn](const AActor& A, const AActor& B) {
        return A.GetDistanceTo(ControlledPawn) < B.GetDistanceTo(ControlledPawn);
    });
}

void ATDMPlayerController::SwitchTarget(int32 Direction)
{
    APawn* ControlledPawn = GetPawn<APawn>();
	if(LockCheckedEnemies.Num() < 2 or !IsValid(ControlledPawn)) return;

	LastActor = ThisActor;
    UnHighlightActor(LastActor);

    CurrentLockIndex = (CurrentLockIndex + Direction + LockCheckedEnemies.Num()) % LockCheckedEnemies.Num();
    if (ControlledPawn->Implements<UCombatInterface>() and
        ICombatInterface::Execute_CanSeeActor(ControlledPawn, LockCheckedEnemies[CurrentLockIndex])) // 如果下一个锁定的敌人在视线中，则更新ThisActor
    {
        ThisActor = LockCheckedEnemies[CurrentLockIndex];
        HighlightActor(ThisActor);
    }
    else
    {
        FindPotentialTargets();
        if(LockCheckedEnemies.IsEmpty())
        {
            CancelLock();
            return;
        }
        CurrentLockIndex = 0;
        ThisActor = LockCheckedEnemies[CurrentLockIndex];
        HighlightActor(ThisActor);
    }
    ICombatInterface::Execute_SetCombatTarget(GetPawn(), ThisActor);
}

void ATDMPlayerController::OnSwitchTargetRight()
{
	SwitchTarget(1);
}

void ATDMPlayerController::OnSwitchTargetLeft()
{
	SwitchTarget(-1);
}

void ATDMPlayerController::UpdateRotation(float DeltaTime)
{
    
	if(bIsLocked and IsValid(ThisActor))
    {
		
        // 锁定状态下：强制看向目标
        APawn* ControlledPawn = GetPawn<APawn>();
        if(ControlledPawn->Implements<UCombatInterface>() and ICombatInterface::Execute_CanSeeActor(ControlledPawn, ThisActor))
        {
			// 计算旋转角度以看向目标Actor
			FRotator LookAtRotation;
            ICombatInterface::Execute_LookAtTarget(ControlledPawn, ThisActor, LookAtRotation);
            float YawDelta = FMath::FindDeltaAngleDegrees(GetControlRotation().Yaw, LookAtRotation.Yaw);
            
            AddYawInput(YawDelta / 100.f);
            
        }
        else
        {
            LockCheckedEnemies.Empty();
            CancelLock();
        }
        
    }
    Super::UpdateRotation(DeltaTime);
}

void ATDMPlayerController::CancelLock()
{
    if (ThisActor)
    {
        UnHighlightActor(ThisActor);
        ThisActor = nullptr;
    }
    if (LastActor)
    {
        UnHighlightActor(LastActor);
        LastActor = nullptr;
    }
    CurrentLockIndex = INDEX_NONE;
    bIsLocked = false;
    if (GetPawn()->Implements<UPlayerInterface>())
    {
        IPlayerInterface::Execute_SetLockRotation(GetPawn(), bIsLocked);
        ICombatInterface::Execute_SetCombatTarget(GetPawn(), nullptr);
    }
}

void ATDMPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ATDMPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void ATDMPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}