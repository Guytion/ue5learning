// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/TeamIdInterface.h"
#include "Weapon.generated.h"

UCLASS()
class THREEDMOBA_API AWeapon : public AActor, public ITeamIdInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int32 TeamId = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "伤害")
	float BaseDamage = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "武器")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "武器")
	UStaticMeshComponent* WeaponMesh;

	UFUNCTION()
	void OnWeaponHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	virtual int32 GetTeamId_Implementation() const override { return TeamId; }

	void ActivateCollision(bool bEnable);

	bool bIsAttacking = false;
};
