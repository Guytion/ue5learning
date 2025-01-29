// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnComponent::USpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
/* void USpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
} */

void USpawnComponent::CrowdSpawn(int32 MaxSpawnAmount, FVector SpawnScope, TSubclassOf<AActor> NPCToSpawn, FRotator SpawnRotation)
{
	const FVector Origin = GetOwner()->GetActorLocation();

	for (int i = 0; i < MaxSpawnAmount; ++i)
	{
		// SpawnScope = UKismetMathLibrary::RandomPointInBoundingBox(Origin, SpawnScope);
		FVector DeltaPos = UKismetMathLibrary::RandomPointInBoundingBox(FVector(0.f, 0.f, 0.f), SpawnScope);
		UE_LOG(LogTemp, Warning, TEXT("位置修正: %s"), *DeltaPos.ToString());
		FVector ProceduralSeperation = Origin + DeltaPos;
		GetWorld()->SpawnActor<AActor>(NPCToSpawn, ProceduralSeperation, SpawnRotation);

	}
}