// Copyright Guytion's Studio


#include "Actors/FireBall.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/TDMAbilitySystemLibrary.h"

void AFireBall::BeginPlay()
{
    Super::BeginPlay();
    StartOutgoingTimeline();
}

void AFireBall::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (HasAuthority() && IsValidOverLap(OtherActor))
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UTDMAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); // 伤害结算
		}
	}
}

void AFireBall::OnHit()
{
    if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}