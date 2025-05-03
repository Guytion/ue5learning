// Copyright Guytion's Studio


#include "AbilitySystem/Abilities/CommonMeleeAttack.h"
#include "Interaction/CombatInterface.h"
#include "ActorComponent/WeaponMeshComponent.h"

void UCommonMeleeAttack::SetWeaponDamage()
{
    if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
    {
        UWeaponMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo());
        if (Weapon)
        {
            Weapon->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
        }
        
    }
}
