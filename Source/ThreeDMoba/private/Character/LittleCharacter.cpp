// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LittleCharacter.h"
#include "ThreeDMoba/Weapon.h"
#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "AbilitySystem/TDMAttributeSet.h"
#include "AbilitySystem/TDMAbilitySystemLibrary.h"

// Sets default values
ALittleCharacter::ALittleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UTDMAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); // 设置复制模式为混合模式(客户端和服务器都处理)

	AttributeSet = CreateDefaultSubobject<UTDMAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void ALittleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

// Called every frame
void ALittleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking && CurrentAttackTime > 0)
	{
		CurrentAttackTime -= DeltaTime;
		if (CurrentAttackTime <= 0)
		{
			bIsAttacking = false;
			CurrentAttackTime = 0;
			AWeapon* Weapon = GetAttachedWeapon();
			Weapon->bIsAttacking = bIsAttacking;
		}
	}
}

// Called to bind functionality to input
void ALittleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALittleCharacter::Attack()
{
	if (AttackMontage)
	{
		bIsAttacking = true;
		float AttackMontageDuration = PlayAnimMontage(AttackMontage);
		AWeapon* Weapon = GetAttachedWeapon();
		Weapon->bIsAttacking = bIsAttacking;
	}
}

AWeapon* ALittleCharacter::GetAttachedWeapon() const
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

float ALittleCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ALittleCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTDMAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
    {
        InitializeDefaultAttributes();
    }
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
}

void ALittleCharacter::InitializeDefaultAttributes() const
{
	UTDMAbilitySystemLibrary::InitializeDefaultAttributes(this, AbilitySystemComponent);
}