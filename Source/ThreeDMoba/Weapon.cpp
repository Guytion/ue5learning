// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("默认场景组件"));
	RootComponent = DefaultSceneRoot;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("武器外观"));
	WeaponMesh->SetupAttachment(DefaultSceneRoot);
	WeaponMesh->SetCollisionProfileName("Weapon");
	// WeaponMesh->SetGenerateOverlapEvents(true);
	// WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponHit);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->Implements<UTeamIdInterface>())
	{
		TeamId = ITeamIdInterface::Execute_GetTeamId(GetOwner());
	}
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponHit);
}

// Called every frame
/* void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} */

void AWeapon::ActivateCollision(bool bEnable)
{
	UE_LOG(LogTemp, Warning, TEXT("碰撞状态设置为: %d"), bEnable);
	WeaponMesh->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	WeaponMesh->SetGenerateOverlapEvents(bEnable);
}

void AWeapon::OnWeaponHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor == GetOwner()) return; // 忽略自身
	
	if (bIsAttacking)
	{
		if (OtherActor->Implements<UTeamIdInterface>())
		{
			if (ITeamIdInterface::Execute_GetTeamId(OtherActor) == TeamId) return; // 避免同队伤害

			// 伤害传递
			FPointDamageEvent DamageEvent;
			DamageEvent.Damage = BaseDamage;
			DamageEvent.HitInfo = SweepResult;

			OtherActor->TakeDamage(BaseDamage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
		}
	}
}