// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeDMobaCharacter.h"
#include "LittleCharacter.generated.h"

class UGameplayAbility;

UCLASS()
class THREEDMOBA_API ALittleCharacter : public AThreeDMobaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALittleCharacter();

	/* Combat Interface  */
	virtual void Die(const FVector& DeathImpulse) override;
	/* Combat Interface 结束 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "小兵|尸体消失时间")
	float LifeSpan = 5.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override; // 初始化默认属性

	void GiveStartupAbilities();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 攻击敌人 **/
	UFUNCTION(BlueprintCallable)
	void Attack();

	class AWeapon* GetAttachedWeapon() const;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	UPROPERTY(EditAnywhere, Category = "小兵|技能", meta = (DisplayName = "起始技能组"))
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
