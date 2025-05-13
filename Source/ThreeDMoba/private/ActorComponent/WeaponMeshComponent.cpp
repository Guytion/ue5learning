// Copyright Guytion's Studio


#include "ActorComponent/WeaponMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/TDMAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UWeaponMeshComponent::BeginPlay()
{
    Super::BeginPlay();
    OnComponentBeginOverlap.AddDynamic(this, &UWeaponMeshComponent::OnOverlap);
    // OnComponentEndOverlap.AddDynamic(this, &UWeaponMeshComponent::OnEndOverlap);
}

void UWeaponMeshComponent::OnOverlap_Implementation(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
) {
    if (IsValidOverlap(OtherActor))
    {
        WeaponHitResult = SweepResult;
        if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
        {
            DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
            UTDMAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams); // 伤害结算
        }
    }
}

bool UWeaponMeshComponent::IsValidOverlap(AActor* OtherActor)
{
    if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
    // if (UTDMAbilitySystemLibrary::IsFriendly(SourceAvatarActor, OtherActor)) return false;
    return true;
}