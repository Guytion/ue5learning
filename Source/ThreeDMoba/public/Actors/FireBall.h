// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile.h"
#include "FireBall.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "火球"))
class THREEDMOBA_API AFireBall : public AProjectile
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "启动往返时间轴"))
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly, Category = "火焰爆发", meta = (DisplayName = "玩家位置"))
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "爆炸伤害参数"))
	FDamageEffectParams ExplosionDamageParams;
	
protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

	virtual void OnHit() override;

private:

	float LastOverlapTime = 0.0f;
	float OverlapDuration = 0.1f; // 0.1秒碰撞间隔时间
};
