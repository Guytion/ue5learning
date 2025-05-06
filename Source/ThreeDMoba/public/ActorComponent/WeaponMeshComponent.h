// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TDMAbilityTypes.h"
#include "WeaponMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UWeaponMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "损伤效果参数"))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(BlueprintReadOnly)
	FHitResult WeaponHitResult;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/* UFUNCTION(Server, Reliable)
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	); */

	bool IsValidOverlap(AActor* OtherActor);
};
