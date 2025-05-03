// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamIdInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeamIdInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THREEDMOBA_API ITeamIdInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "队伍", meta = (DisplayName = "获取队伍ID"))
	int32 GetTeamId() const;

	UFUNCTION(BlueprintNativeEvent, Category = "队伍", meta = (DisplayName = "设置队伍ID"))
	void SetTeamId(int32 NewTeamId);

};
