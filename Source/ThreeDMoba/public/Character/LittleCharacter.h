// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeDMobaCharacter.h"
#include "LittleCharacter.generated.h"

UCLASS()
class THREEDMOBA_API ALittleCharacter : public AThreeDMobaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALittleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual int32 GetTeamId_Implementation() const override { return TeamId; }

	/** 攻击敌人 **/
	UFUNCTION(BlueprintCallable)
	void Attack();

	class AWeapon* GetAttachedWeapon() const;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	float CurrentAttackTime = 0;

};
