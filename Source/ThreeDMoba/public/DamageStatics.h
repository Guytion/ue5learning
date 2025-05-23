// Copyright Guytion's Studio

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/TDMAttributeSet.h"

struct TDMDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(MeleeAttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(RangedAttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MagicAttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MissChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);

    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(WaterResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(EarthResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);

    TDMDamageStatics();

    static const TDMDamageStatics& Get();
};

// 全局访问函数
FORCEINLINE const TDMDamageStatics& DamageStatics()
{
    return TDMDamageStatics::Get();
}