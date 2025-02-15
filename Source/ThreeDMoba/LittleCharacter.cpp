// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleCharacter.h"
#include "Weapon.h"
#include "HealthComponent.h"

// Sets default values
ALittleCharacter::ALittleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("生命值"));
}

// Called when the game starts or when spawned
void ALittleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	// 扣除生命值
	HealthComponent->LoseHealth(ActualDamage);

	return ActualDamage;
}

void ALittleCharacter::OnDeath_Implementation()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors); // 获取所有附加的Actor
	for (AActor* Actor : AttachedActors)
	{
		Actor->Destroy(); // 销毁附加的Actor
	}
	// 播放死亡动画
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	// 销毁角色
	Destroy();
}