// Copyright Guytion's Studio


#include "TDMGameplayTags.h"
#include "GameplayTagsManager.h"

FTDMGameplayTags FTDMGameplayTags::GameplayTags; // 定义静态变量并初始化

void FTDMGameplayTags::InitializeNativeGameplayTags()
{
    GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString(TEXT("力量")));
    GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString(TEXT("智力"))); // 添加标签并返回引用
    GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Agility"), FString(TEXT("敏捷"))); // 添加标签并返回引用

    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString(TEXT("最大生命值")));
    GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString(TEXT("最大魔法值")));
    GameplayTags.Attributes_Secondary_QiAccumulation = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.QiAccumulation"), FString(TEXT("气积累速度"))); // 添加标签并返回引用
    GameplayTags.Attributes_Secondary_MeleeAttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MeleeAttackPower"), FString(TEXT("近战攻击力")));
    GameplayTags.Attributes_Secondary_RangedAttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.RangedAttackPower"), FString(TEXT("远程攻击力"))); // 添加标签并返回引用
    GameplayTags.Attributes_Secondary_MagicAttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicAttackPower"), FString(TEXT("魔法攻击力"))); // 添加标签并返回引用
    GameplayTags.Attributes_Secondary_MissChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MissChance"), FString(TEXT("闪避几率"))); // 添加标签并返回引用
    GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString(TEXT("暴击几率"))); // 添加标签并返回引用
    GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString(TEXT("生命恢复"))); // 添加标签并返回引用
    GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString(TEXT("法力恢复"))); // 添加标签并返回引用

    /* 输入标签 */
    GameplayTags.InputTag_KeyMouse_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.KeyMouse.LMB"), FString(TEXT("鼠标左键")));
    GameplayTags.InputTag_KeyMouse_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.KeyMouse.1"), FString(TEXT("1键")));
    
    GameplayTags.InputTag_Joystick_Xbox_RT = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Joystick.Xbox_RT"), FString(TEXT("游戏手柄右板机轴")));
    GameplayTags.InputTag_Joystick_Xbox_X = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Joystick.Xbox_X"), FString(TEXT("游戏手柄正面按钮左")));
    /* 输入标签结束*/

    /* 伤害类型 */
    GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString(TEXT("伤害")));
    GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString(TEXT("物理伤害")));
    GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString(TEXT("火焰伤害")));
    GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString(TEXT("闪电伤害")));
    GameplayTags.Damage_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Water"), FString(TEXT("水系伤害")));
    GameplayTags.Damage_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Earth"), FString(TEXT("土系伤害")));
    GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString(TEXT("奥术伤害")));
    /* 伤害类型结束 */

    /* 抗性类型 */
    GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString(TEXT("物理防御")));
    GameplayTags.Attributes_Resistance_BasicMagic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.BasicMagic"), FString(TEXT("基础魔法抗性")));
    GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString(TEXT("火焰抗性")));
    GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString(TEXT("闪电抗性")));
    GameplayTags.Attributes_Resistance_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Water"), FString(TEXT("水系抗性")));
    GameplayTags.Attributes_Resistance_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Earth"), FString(TEXT("土系抗性")));
    GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString(TEXT("奥术抗性")));
    /* 抗性类型结束 */

    /* 元属性 */
    GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString(TEXT("即将获得的XP")));
    /* 元属性结束 */

    /* 技能 */
    GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString(TEXT("空技能")));
    GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString(TEXT("普通攻击")));
    /* 技能结束 */

    GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString(TEXT("击中反应能力")));

    /* 被动技能 */
    GameplayTags.Abilities_Passive_ListenForEvent = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ListenForEvent"), FString(TEXT("监听事件")));
    /* 被动技能结束 */

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

    /* 游戏效果 */
    GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString(TEXT("击退反应")));
    /* 游戏效果结束 */

    /* GameplayCues */
    GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.MeleeImpact"), FString(TEXT("近战冲击")));
    /* GameplayCues 结束 */

    /* 伤害与抗性的对应关系 */
    GameplayTags.DamageTypestoResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
    GameplayTags.DamageTypestoResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
    GameplayTags.DamageTypestoResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
    GameplayTags.DamageTypestoResistances.Add(GameplayTags.Damage_Water, GameplayTags.Attributes_Resistance_Water);
    GameplayTags.DamageTypestoResistances.Add(GameplayTags.Damage_Earth, GameplayTags.Attributes_Resistance_Earth);
    GameplayTags.DamageTypestoResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
    /* 伤害与抗性的对应关系结束 */
}