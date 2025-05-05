// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAbilitySystemLibrary.h"
#include "Interaction/TeamIdInterface.h"
#include "GameplayEffectTypes.h"
#include "TDMGameplayTags.h"
#include "TDMAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UTDMAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
    /* UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (ClassInfo == nullptr) return;
    
    AActor* AvatarActor = ASC->GetAvatarActor();
    
    FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

    FGameplayEffectContextHandle AttributesContextHandle = ASC->MakeEffectContext();
    AttributesContextHandle.AddSourceObject(AvatarActor);

    const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, AttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

    const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes_Enemy, Level, AttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, AttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get()); */
}

void UTDMAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
    
}

bool UTDMAbilitySystemLibrary::IsFriendly(AActor* ActorA, AActor* ActorB)
{
    if (ActorA->Implements<UTeamIdInterface>() && ActorB->Implements<UTeamIdInterface>())
    {
        return ITeamIdInterface::Execute_GetTeamId(ActorA) == ITeamIdInterface::Execute_GetTeamId(ActorB);
    }
    return false;
}

FGameplayEffectContextHandle UTDMAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    const FTDMGameplayTags& GameplayTags = FTDMGameplayTags::Get();
    const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

    FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContexthandle.AddSourceObject(SourceAvatarActor);

    const FGameplayEffectSpecHandle DamageSpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
        DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage); // BaseDamage随等级变化
    
    DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);
    return EffectContexthandle;
}