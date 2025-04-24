// Fill out your copyright notice in the Description page of Project Settings.


#include "TDMAssetManager.h"
#include "TDMGameplayTags.h"

UTDMAssetManager& UTDMAssetManager::Get()
{
    check(GEngine);

    UTDMAssetManager* Manager = Cast<UTDMAssetManager>(GEngine->AssetManager);
    return *Manager;
}

void UTDMAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    FTDMGameplayTags::InitializeNativeGameplayTags(); // 初始化游戏标签
}