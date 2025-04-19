// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAbilitySystemLibrary.h"

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