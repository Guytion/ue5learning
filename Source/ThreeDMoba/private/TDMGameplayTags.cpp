// Copyright Guytion's Studio


#include "TDMGameplayTags.h"
#include "GameplayTagsManager.h"

FTDMGameplayTags FTDMGameplayTags::GameplayTags; // 定义静态变量并初始化

void FTDMGameplayTags::InitializeNativeGameplayTags()
{
    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString(TEXT("玩家死亡前所能承受的最大伤害")));
    GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString(TEXT("玩家释放技能的最大消耗")));

    /* 输入标签 */
    GameplayTags.InputTag_KeyMouse_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.KeyMouse.LMB"), FString(TEXT("鼠标左键")));
    
    GameplayTags.InputTag_Joystick_Xbox_X = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Joystick.Xbox_X"), FString(TEXT("游戏手柄正面按钮左")));
    /* 输入标签结束*/

    /* 伤害类型 */
    GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString(TEXT("伤害")));
    GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString(TEXT("物理伤害")));
    /* 伤害类型结束*/

    GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString(TEXT("击中反应能力")));

    /* 技能状态 */
    GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString(TEXT("锁定状态")));
    GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString(TEXT("可用状态")));
    GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString(TEXT("解锁状态")));
    /* 技能状态结束 */

    /* 战斗插槽 */
    GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString(TEXT("武器")));
    GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString(TEXT("右手")));
    GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString(TEXT("左手")));
    /* 战斗插槽结束 */

    /* 蒙太奇 */
    GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString(TEXT("攻击1")));
    GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString(TEXT("攻击2")));
    GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString(TEXT("攻击3")));
    GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString(TEXT("攻击4")));
    /* 蒙太奇结束 */
}