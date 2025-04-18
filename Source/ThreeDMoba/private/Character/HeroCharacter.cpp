// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AHeroCharacter::AHeroCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

AWeapon* AHeroCharacter::GetAttachedWeapon() const
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors); // 获取所有附加的Actor
	for (AActor* Actor : AttachedActors)
	{
		if (AWeapon* Weapon = Cast<AWeapon>(Actor))
		{
			return Weapon;
		}
	}
	return nullptr;
}

bool AHeroCharacter::CanSeeActor(const AActor* TargetActor) const
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