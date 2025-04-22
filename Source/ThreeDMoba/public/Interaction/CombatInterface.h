// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THREEDMOBA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	bool CanSeeActor(const AActor* TargetActor) const;

	UFUNCTION(BlueprintNativeEvent)
	void LookAtTarget(const AActor* TargetActor, FRotator& LookAtRotation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "面向目标"))
	void UpdateFacingTarget(const FVector& Target);

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;

};
