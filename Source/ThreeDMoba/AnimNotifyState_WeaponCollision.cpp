// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_WeaponCollision.h"
#include "Character/LittleCharacter.h"
#include "Weapon.h"

void UAnimNotifyState_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (ALittleCharacter* Character = Cast<ALittleCharacter>(MeshComp->GetOwner()))
    {
        if (AWeapon* Weapon = Character->GetAttachedWeapon())
        {
            Weapon->ActivateCollision(true);
        }
    }
    
}

void UAnimNotifyState_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (ALittleCharacter* Character = Cast<ALittleCharacter>(MeshComp->GetOwner()))
    {
        if (AWeapon* Weapon = Character->GetAttachedWeapon())
        {
            Weapon->ActivateCollision(false);
        }
    }
}