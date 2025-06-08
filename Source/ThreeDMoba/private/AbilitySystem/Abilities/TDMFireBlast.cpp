// Copyright Guytion's Studio


#include "AbilitySystem/Abilities/TDMFireBlast.h"
#include "AbilitySystem/TDMAbilitySystemLibrary.h"
#include "Actors/FireBall.h"

TArray<AFireBall*> UTDMFireBlast::SpawnFireBalls()
{
    TArray<AFireBall*> FireBalls;
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    TArray<FRotator> Rotators = UTDMAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

    for (const FRotator& Rotator : Rotators)
    {
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(Location);
        SpawnTransform.SetRotation(Rotator.Quaternion());
        AFireBall* FireBall = GetWorld()->SpawnActorDeferred<AFireBall>(
            FireBallClass, 
            SpawnTransform,
            GetOwningActorFromActorInfo(),
            CurrentActorInfo->PlayerController->GetPawn(),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

        FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
        FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
        FireBall->SetOwner(GetAvatarActorFromActorInfo());

        FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();

        FireBalls.Add(FireBall);
        FireBall->FinishSpawning(SpawnTransform); // 完成生成，设置为可交互状态
    }
    
    return FireBalls;
}