// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "Player/TDMPlayerState.h"
#include "Player/TDMPlayerController.h"
#include "UI/HUD/PlayerHUD.h"
#include "UI/Widget/TDMUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	HeroStatusBar = CreateDefaultSubobject<UTDMWidgetComponent>(TEXT("HeroStatusBar"));
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
	
	HeroStatusBar->InitWidget();
	// BroadcastInitialValues();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
    
    // 在客户端上初始化玩家状态
    InitAbilityActorInfo();

	HeroStatusBar->InitWidget();
	// BroadcastInitialValues();
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	HeroStatusBar->SetVisibility(!IsLocallyControlled());
	
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
		/* BindCallbacksToDependencies();
		if (UTDMUserWidget* TDMUserWidget = Cast<UTDMUserWidget>(HeroStatusBar->GetUserWidgetObject()))
		{
			TDMUserWidget->SetWidgetController(this); // 触发WidgetControllerSet事件
		} */
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

/* void AHeroCharacter::BindCallbacksToDependencies()
{
	if (AbilitySystemComponent && AttributeSet)
	{
		const UTDMAttributeSet* HeroAS = CastChecked<UTDMAttributeSet>(AttributeSet);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HeroAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HeroAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HeroAS->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HeroAS->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);
	}
}

void AHeroCharacter::BroadcastInitialValues()
{
	if (const UTDMAttributeSet* HeroAS = Cast<UTDMAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(HeroAS->GetHealth());
		OnMaxHealthChanged.Broadcast(HeroAS->GetMaxHealth());
		OnManaChanged.Broadcast(HeroAS->GetMana());
		OnMaxManaChanged.Broadcast(HeroAS->GetMaxMana());
	}
} */