// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/TeamIdInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemInterface.h"
#include "ThreeDMobaCharacter.generated.h"

class UGameplayEffect;
class UAttributeSet;

UCLASS(config=Game)
class AThreeDMobaCharacter : public ACharacter,
public ITeamIdInterface,
public IAbilitySystemInterface,
public ICombatInterface
{
	GENERATED_BODY()

public:

	AThreeDMobaCharacter();

	virtual void Jump() override;

	UFUNCTION(Server, Reliable)
	void ServerJump();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* TeamId Interface */
	virtual int32 GetTeamId_Implementation() const override { return TeamId; }
	virtual void SetTeamId_Implementation(int32 NewTeamId) override { TeamId = NewTeamId; }
	/* TeamId Interface 结束 */

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override; // AbilitySystemInterface
	UAttributeSet* GetAttributeSet() const { return AttributeSet; } // 获取属性集

	/* Combat Interface  */
	virtual bool CanSeeActor_Implementation(const AActor* TargetActor) const override;
	virtual void LookAtTarget_Implementation(AActor* TargetActor, FRotator& LookAtRotation) override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override; // 获取角色技能系统组件注册事件
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	virtual void SetIsAttacking_Implementation(bool IsAttacking) override;
	virtual UWeaponMeshComponent* GetWeapon_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	/* Combat Interface 结束 */

	FOnASCRegistered OnASCRegisteredDelegate; // 角色技能系统组件注册事件

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|队伍")
	int32 TeamId = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* GetHitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* DeathMontage;
	
	virtual void BeginPlay();

	virtual void InitAbilityActorInfo();
	virtual void InitializeDefaultAttributes() const; // 初始化默认属性
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	UPROPERTY(meta = (DisplayName = "技能系统组件"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; // 技能系统组件

	UPROPERTY(meta = (DisplayName = "属性集"))
	TObjectPtr<UAttributeSet> AttributeSet; // 属性集

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|属性", meta = (DisplayName = "主要属性"))
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|属性", meta = (DisplayName = "次生属性"))
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|属性", meta = (DisplayName = "关键属性"))
	TSubclassOf<UGameplayEffect> InitVitalAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "角色|武器", meta = (DisplayName = "武器"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "角色|战斗", meta = (DisplayName = "普通攻击动画蒙太奇"))
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|武器", meta = (DisplayName = "武器战斗插槽"))
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|战斗", meta = (DisplayName = "左手战斗插槽"))
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|战斗", meta = (DisplayName = "右手战斗插槽"))
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = "角色|战斗", meta = (DisplayName = "受击反应动画蒙太奇"))
	TObjectPtr<UAnimMontage> HitReactMontage;

	// 网络同步

	/**
	 * @brief 设置角色朝向目标（网络同步）
	 * @param Rotation 目标旋转角度
	 */
	UFUNCTION(Server, Reliable)
	void ServerRotateToTarget(const FRotator& Rotation);

	UPROPERTY(ReplicatedUsing = OnRep_CharacterRotation)
	FRotator CharacterRotation;

	UFUNCTION()
	void OnRep_CharacterRotation();

private:

	UPROPERTY(ReplicatedUsing = OnRep_CombatTarget)
	TObjectPtr<AActor> CombatTarget;

	// 网络同步

	UFUNCTION()
	void OnRep_CombatTarget();

	UFUNCTION(Server, Reliable)
	void ServerSetCombatTarget(AActor* InCombatTarget);
};

