// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "TDMPlayerState.generated.h"

class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* 状态值 */);
/**
 * 
 */
UCLASS()
class THREEDMOBA_API ATDMPlayerState : public APlayerState,
public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ATDMPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FOnPlayerStatChanged OnXPChangedDelegate; // 玩家经验变化委托
	FOnPlayerStatChanged OnLevelChangedDelegate; // 玩家等级变化委托
	FOnPlayerStatChanged OnAttributePointsChangedDelegate; // 属性点变化委托
	FOnPlayerStatChanged OnSpellPointsChangedDelegate; // 技能点变化委托

	FORCEINLINE int32 GetPlayerLevel() const { return Level; } // 获取玩家等级
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);

	UPROPERTY(EditDefaultsOnly, Category = "角色升级信息")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	void AddToAttributePoints(int32 InAttributePoints);
	void SetAttributePoints(int32 InAttributePoints);
	
	void AddToSpellPoints(int32 InSpellPoints);
	void SetSpellPoints(int32 InSpellPoints);

	UAbilitySystemComponent* GetAbilitySystemComponent() const override; // AbilitySystemInterface
	UAttributeSet* GetAttributeSet() const { return AttributeSet; } // 获取属性集

protected:

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "技能系统组件"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; // 能力系统组件

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; // 属性集

private:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_Level, meta = (DisplayName = "玩家等级"))
	int32 Level = 1; // 玩家等级

	UFUNCTION()
	void OnRep_Level(int32 OldLevel); // 等级更新的回调函数

	UPROPERTY(ReplicatedUsing=OnRep_XP)
	int32 XP = 0; // 经验值

	UFUNCTION()
	void OnRep_XP(int32 OldXP); // 经验值更新的回调函数

	UPROPERTY(ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0; // 属性点

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints); // 属性点更新的回调函数

	UPROPERTY(ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0; // 技能点

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints); // 技能点更新的回调函数
};
