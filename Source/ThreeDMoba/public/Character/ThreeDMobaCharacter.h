// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/TeamIdInterface.h"
#include "AbilitySystemInterface.h"
#include "ThreeDMobaCharacter.generated.h"

class UGameplayEffect;
class UAttributeSet;

UCLASS(config=Game)
class AThreeDMobaCharacter : public ACharacter,
public ITeamIdInterface,
public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AThreeDMobaCharacter();

	virtual int32 GetTeamId_Implementation() const override { return TeamId; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override; // AbilitySystemInterface

protected:

	// TArray<AActor*> LockCheckedEnemies;

	void LookAtTarget(const AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int32 TeamId = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* GetHitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* DeathMontage;
	
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual void InitAbilityActorInfo();
	virtual void InitializeDefaultAttributes() const; // 初始化默认属性

	UPROPERTY(meta = (DisplayName = "技能系统组件"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; // 技能系统组件

	UPROPERTY(meta = (DisplayName = "属性集"))
	TObjectPtr<UAttributeSet> AttributeSet; // 属性集

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色属性", meta = (DisplayName = "主要属性"))
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
};

