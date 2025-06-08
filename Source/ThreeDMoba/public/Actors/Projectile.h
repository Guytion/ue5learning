// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDMAbilityTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class UNiagaraSystem;

UCLASS(meta = (DisplayName = "发射物"))
class THREEDMOBA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true, DisplayName = "损伤效果参数"))
	FDamageEffectParams DamageEffectParams; // 损伤效果参数

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComp; // 导弹追踪目标组件，用于无目标时被垃圾回收

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "发射物移动组件"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement; // 发射物移动组件

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "碰撞球体"))
	TObjectPtr<USphereComponent> Sphere;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	bool bHit = false;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	bool IsValidOverLap(AActor* OtherActor);

	virtual void Destroyed() override;

private:

	UPROPERTY(EditAnywhere, meta = (DisplayName = "撞击特效"))
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "撞击声效"))
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "循环声效"))
	TObjectPtr<USoundBase> LoopingSound;
};
