// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAbilitySystemGlobals.h"
#include "TDMAbilityTypes.h"

FGameplayEffectContext* UTDMAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FTDMGameplayEffectContext(); // 创建自定义的GameplayEffectContext上下文对象，并返回该对象的指针。这样，我们就可以在游戏中使用自定义的GameplayEffectContext上下文对象了。
}