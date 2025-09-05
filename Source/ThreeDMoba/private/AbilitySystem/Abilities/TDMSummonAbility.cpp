// Copyright Guytion's Studio


#include "AbilitySystem/Abilities/TDMSummonAbility.h"

TArray<FVector> UTDMSummonAbility::GetSpawnLocation()
{
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    const float DeltaSpread = SpawnSpread / NumMinions;

    const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
    TArray<FVector> SpawnLocations;
    for (int32 i = 0; i < NumMinions; i++)
    {
        const FVector Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * i, FVector::UpVector);
        FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

        FHitResult Hit;
        // 找到在地面上的生成位置
        GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
        if (Hit.bBlockingHit)
        {
            ChosenSpawnLocation = Hit.ImpactPoint;
        }
        SpawnLocations.Add(ChosenSpawnLocation);
    }

    return SpawnLocations;
}

TSubclassOf<AActor> UTDMSummonAbility::GetRandomMinionClass()
{
    const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
    return MinionClasses[Selection];
}