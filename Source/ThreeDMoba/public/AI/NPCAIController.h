// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// 初始化函数
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPCAIController")
	UBehaviorTree* BTAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerComp; // AI感知组件

	class UAISenseConfig_Sight* SightConfig; // 视觉感知配置

	UFUNCTION() // 必须添加此宏，否则委托绑定失败
	void OnTargetPerceptionUpdated(AActor* Actor, const struct FAIStimulus Stimulus);

public:

	ANPCAIController();
};
