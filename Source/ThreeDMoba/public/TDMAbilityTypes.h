// Copyright Guytion's Studio

#pragma once

#include "GameplayEffectTypes.h"
#include "TDMAbilityTypes.generated.h"

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
		return NewContext;
	}
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