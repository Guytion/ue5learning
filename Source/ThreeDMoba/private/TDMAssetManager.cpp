// Fill out your copyright notice in the Description page of Project Settings.


#include "TDMAssetManager.h"

UTDMAssetManager& UTDMAssetManager::Get()
{
    check(GEngine);

    UTDMAssetManager* Manager = Cast<UTDMAssetManager>(GEngine->AssetManager);
    return *Manager;
}