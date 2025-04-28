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

    FGameplayTag Abilities_HitReact;
	
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;

    FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

    FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

private:

    static FTDMGameplayTags GameplayTags;

};
