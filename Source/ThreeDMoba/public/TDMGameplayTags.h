// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TDMGameplayTags
 * 
 * 包含原生游戏标签的单例
 */

struct FTDMGameplayTags
{
public:

    static const FTDMGameplayTags& Get() { return GameplayTags; }

    static void InitializeNativeGameplayTags();

    /* 键鼠输入 */
    FGameplayTag InputTag_KeyMouse_LMB;
    /* 键鼠输入 结束 */

    /* 手柄输入 */
    FGameplayTag InputTag_Joystick_Xbox_X;
    /* 手柄输入 结束 */

    FGameplayTag Damage;
    FGameplayTag Damage_Physical;
    FGameplayTag Damage_Fire;
    FGameplayTag Damage_Lightning;
    FGameplayTag Damage_Water;
    FGameplayTag Damage_Earth;
    FGameplayTag Damage_Arcane;

    FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;

    FGameplayTag Abilities_HitReact;
	
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;

    FGameplayTag Abilities_Passive_ListenForEvent;

    FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

    FGameplayTag Attributes_Primary_Strength;
    FGameplayTag Attributes_Primary_Intelligence;
    FGameplayTag Attributes_Primary_Agility;

    FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
    FGameplayTag Attributes_Secondary_QiAccumulation;
    FGameplayTag Attributes_Secondary_MeleeAttackPower;
    FGameplayTag Attributes_Secondary_RangedAttackPower;
    FGameplayTag Attributes_Secondary_MagicAttackPower;
    FGameplayTag Attributes_Secondary_MissChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;

    FGameplayTag Attributes_Resistance_Physical;
    FGameplayTag Attributes_Resistance_BasicMagic;
    FGameplayTag Attributes_Resistance_Fire;
    FGameplayTag Attributes_Resistance_Lightning;
    FGameplayTag Attributes_Resistance_Water;
    FGameplayTag Attributes_Resistance_Earth;
    FGameplayTag Attributes_Resistance_Arcane;

    FGameplayTag Attributes_Meta_IncomingXP;

    FGameplayTag Effects_HitReact;

    FGameplayTag GameplayCue_MeleeImpact;

    TArray<FGameplayTag> DamageTypes;
	TMap<FGameplayTag, FGameplayTag> DamageTypestoResistances; // 损伤类型到抗性的映射

private:

    static FTDMGameplayTags GameplayTags;

};
