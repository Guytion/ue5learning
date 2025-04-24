// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TDMAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API UTDMAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UTDMAssetManager& Get();

protected:

	virtual void StartInitialLoading() override; // 覆盖虚函数，项目启动时加载初始资源
};
