// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THREEDMOBA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	bool CanSeeActor(const AActor* TargetActor) const;

	UFUNCTION(BlueprintNativeEvent)
	void LookAtTarget(AActor* TargetActor, FRotator& LookAtRotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "面向目标"))
	void UpdateFacingTarget(const FVector& Target);

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "设置战斗目标"))
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "获取战斗目标"))
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "获取普通攻击动画蒙太奇数组"))
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "获取战斗插槽位置"))
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsAttacking(bool IsAttacking);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "获取持有武器"))
	UWeaponMeshComponent* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "获取击打反应动画蒙太奇"))
	UAnimMontage* GetHitReactMontage(); // 获取击打反应动画 montage

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	virtual int32 GetCharacterLevel() const = 0;
	virtual void Die(const FVector& DeathImpulse) = 0; // 死亡逻辑，纯虚函数，必须实现

	virtual FOnDeath& GetOnDeathDelegate() = 0; // 返回值需为地址引用

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
};
