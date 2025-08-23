// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LevelUpComponent.generated.h"

/**
 * 
 */
UCLASS()
class THREEDMOBA_API ULevelUpComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "升级事件"))
	void LevelUp();
};
