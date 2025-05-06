// Copyright Guytion's Studio


#include "AbilitySystem/Abilities/CommonMeleeAttack.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "ActorComponent/WeaponMeshComponent.h"

void UCommonMeleeAttack::SetWeaponDamage()
{
    if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
    {
        UWeaponMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo());
        if (Weapon)
        {
            Weapon->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
            Weapon->WeaponHitResult = FHitResult(); // 清除击中结果
        }
    }
}

float UCommonMeleeAttack::GetDamageAtLevel() const
{
    if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>()) // 如果是英雄，返回正常伤害值
    {
        return Damage.GetValueAtLevel(GetAbilityLevel());
    }
    else // 如果是小兵，伤害值降为1/10
    {
        return Damage.GetValueAtLevel(GetAbilityLevel()) / 10;
    }
}