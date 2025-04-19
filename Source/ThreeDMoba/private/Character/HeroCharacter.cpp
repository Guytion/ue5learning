// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "Player/TDMPlayerState.h"
#include "Player/TDMPlayerController.h"
#include "UI/HUD/PlayerHUD.h"

AHeroCharacter::AHeroCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

    // 在服务器上初始化玩家状态
    InitAbilityActorInfo();

	AddCharacterAbilities();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
    
    // 在客户端上初始化玩家状态
    InitAbilityActorInfo();
}

AWeapon* AHeroCharacter::GetAttachedWeapon() const
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors); // 获取所有附加的Actor
	for (AActor* Actor : AttachedActors)
	{
		if (AWeapon* Weapon = Cast<AWeapon>(Actor))
		{
			return Weapon;
		}
	}
	return nullptr;
}

bool AHeroCharacter::CanSeeActor(const AActor* TargetActor) const
{
	if (!TargetActor)
	{
		return false;
	}
	FHitResult Hit;

	FVector Start = GetActorLocation();
	FVector End = TargetActor->GetActorLocation();
	float Distance = FVector::DistXY(Start, End);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	float DeltaAngleDegrees = FMath::FindDeltaAngleDegrees(FollowCamera->GetComponentRotation().Yaw, LookAtRotation.Yaw);
	if (DeltaAngleDegrees < 70.f && DeltaAngleDegrees > -70.f && Distance < 1000.f)
	{
		ECollisionChannel Channel = ECC_Visibility; // 使用Visibility通道进行射线检测
	
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 忽略自身
		QueryParams.AddIgnoredActor(TargetActor); // 忽略目标Actor

		// 执行射线检测
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
		return !Hit.bBlockingHit; // 如果没有被阻挡，则可以看见目标Actor
	}
	else
	{
		return false; // 如果角度超出范围，则无法看见目标Actor
	}
}

void AHeroCharacter::AddCharacterAbilities()
{
	UTDMAbilitySystemComponent* HeroASC = CastChecked<UTDMAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!HasAuthority()) return;

	HeroASC->AddCharacterAbilities(StartupAbilities);
	HeroASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AHeroCharacter::InitAbilityActorInfo()
{
	if (ATDMPlayerState* HeroPS = GetPlayerState<ATDMPlayerState>())
	{
		AbilitySystemComponent = HeroPS->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(HeroPS, this);
		Cast<UTDMAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
		AttributeSet = HeroPS->GetAttributeSet(); // 初始化属性集
		
		if (ATDMPlayerController* HeroPC = Cast<ATDMPlayerController>(GetController()))
		{
			if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(HeroPC->GetHUD()))
			{
				PlayerHUD->InitOverlay(HeroPC, HeroPS, AbilitySystemComponent, AttributeSet);
			}
		}
	}
}

void AHeroCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
}

void AHeroCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}