// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/TeamIdInterface.h"
#include "ThreeDMobaCharacter.generated.h"

UCLASS(config=Game)
class AThreeDMobaCharacter : public ACharacter,
public ITeamIdInterface
{
	GENERATED_BODY()

public:

	AThreeDMobaCharacter();

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

protected:
	
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:

	virtual int32 GetTeamId_Implementation() const override { return TeamId; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

