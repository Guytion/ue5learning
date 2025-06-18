// Copyright Guytion's Studio

#pragma once

#include "GameplayEffectTypes.h"
#include "TDMAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType, meta = (DisplayName = "伤害效果参数"))
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FTDMGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:

    /** 自定义序列化 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

    /** 返回实际用于序列化的结构 */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

    virtual FTDMGameplayEffectContext* Duplicate() const
	{
		FTDMGameplayEffectContext* NewContext = new FTDMGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		if (GetDamageType())
		{
			NewContext->AddDamageType(*GetDamageType());
		}
		
		return NewContext;
	}

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsMissed() const { return bIsMissed; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }

	void AddDamageType(const FGameplayTag& InDamageType);

	void SetIsCriticalHit(bool InIsCriticalHit) { bIsCriticalHit = InIsCriticalHit; }
	void SetIsMissed(bool InIsMissed) { bIsMissed = InIsMissed; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }

protected:

	TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(); // 智能指针已有垃圾回收机制，不能加UPROPERTY宏

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsMissed = false;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits<FTDMGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTDMGameplayEffectContext>
{
    enum
    {
        WithNetSerializer =  true, // 必须设为true以启用网络序列化 
        WithCopy = true, // 结构体可以被复制。
    };
};