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

};
