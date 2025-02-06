// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleCharacter.h"

// Sets default values
ALittleCharacter::ALittleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALittleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/* void ALittleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

} */

// Called to bind functionality to input
void ALittleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

