// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "Player/TDMPlayerState.h"
#include "Player/TDMPlayerController.h"
#include "UI/HUD/PlayerHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/TDMUserWidget.h"

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

	NetPriority = 5.0f; // 提升同步优先级（默认4.0）

	HeroStatusBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeroStatusBar"));
	HeroStatusBar->SetupAttachment(RootComponent);
	HeroStatusBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

    // 在服务器上初始化玩家状态
    InitAbilityActorInfo();

	InitializeDefaultAttributes();
	AddCharacterAbilities();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
    
    // 在客户端上初始化玩家状态
    InitAbilityActorInfo();
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UTDMUserWidget* TDMUserWidget = Cast<UTDMUserWidget>(HeroStatusBar->GetUserWidgetObject()))
	{
		TDMUserWidget->SetWidgetController(this); // 触发WidgetControllerSet事件
	}
	
	if (const UTDMAttributeSet* TDMAS = Cast<UTDMAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDMAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDMAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDMAS->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TDMAS->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(TDMAS->GetHealth());
		OnMaxHealthChanged.Broadcast(TDMAS->GetMaxHealth());
		OnManaChanged.Broadcast(TDMAS->GetMana());
		OnMaxManaChanged.Broadcast(TDMAS->GetMaxMana());

	}
	
}
AWeapon* AHeroCharacter::GetAttachedWeapon() const
{
	/* TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors); // 获取所有附加的Actor
	for (AActor* Actor : AttachedActors)
	{
		if (AWeapon* Weapon = Cast<AWeapon>(Actor))
		{
			return Weapon;
		}
	} */
	return nullptr;
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
		OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent); // 广播ASC注册完成事件
		
		if (ATDMPlayerController* HeroPC = Cast<ATDMPlayerController>(GetController()))
		{
			if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(HeroPC->GetHUD()))
			{
				PlayerHUD->InitOverlay(HeroPC, HeroPS, AbilitySystemComponent, AttributeSet);
			}
		}
	}
}

void AHeroCharacter::HighlightActor_Implementation()
{
	bHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(252);
}

void AHeroCharacter::UnHighlightActor_Implementation()
{
	bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
    GetMesh()->SetCustomDepthStencilValue(0);
}

void AHeroCharacter::SetLockRotation_Implementation(bool bIsLocked)
{
	if (GetCharacterMovement())
    {
        GetCharacterMovement()->bOrientRotationToMovement = !bIsLocked;
    }
}

