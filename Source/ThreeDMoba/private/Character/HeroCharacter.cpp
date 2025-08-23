// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroCharacter.h"
#include "AbilitySystem/TDMAbilitySystemComponent.h"
#include "Player/TDMPlayerState.h"
#include "Player/TDMPlayerController.h"
#include "UI/HUD/PlayerHUD.h"
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

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraComponent"));
    LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
    LevelUpNiagaraComponent->bAutoActivate = false;

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

int32 AHeroCharacter::GetCharacterLevel() const
{
	if (const ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		return TDMPlayerState->GetPlayerLevel();
	}
	else
	{
		return Super::GetCharacterLevel();
	}
}

void AHeroCharacter::InitializeDefaultAttributes() const
{
	Super::InitializeDefaultAttributes();
	ApplyEffectToSelf(AttributesRegeneration, 1.f);
}

int32 AHeroCharacter::GetXP_Implementation() const
{
	if (const ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		return TDMPlayerState->GetXP();
	}
	return 0;
}

void AHeroCharacter::AddToXP_Implementation(int32 InXP)
{
	if (ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		TDMPlayerState->AddToXP(InXP);
	}
}

int32 AHeroCharacter::FindLevelForXP_Implementation(int32 XP) const
{
	if (const ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		return TDMPlayerState->LevelUpInfo->FindLevelForXP(XP);
	}
	return 1;
}

int32 AHeroCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	if (const ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		return TDMPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointsReward;
	}
	return 0;
}

void AHeroCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	if (ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		TDMPlayerState->AddToLevel(InPlayerLevel);
		if (UTDMAbilitySystemComponent* HeroASC = Cast<UTDMAbilitySystemComponent>(GetAbilitySystemComponent()))
		{
			HeroASC->UpdateAbilityStatuses(TDMPlayerState->GetPlayerLevel());
		}
	}
}

void AHeroCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	if (ATDMPlayerState* TDMPlayerState = Cast<ATDMPlayerState>(GetPlayerState()))
	{
		return TDMPlayerState->AddToSpellPoints(InSpellPoints);
	}
}

void AHeroCharacter::LevelUp_Implementation()
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, GetCharacterLevel());
	if (ATDMPlayerController* PlayerController = Cast<ATDMPlayerController>(GetController()))
	{
		PlayerController->ShowLevelup();
	}
	MulticastLevelUpParticles();
}

void AHeroCharacter::MulticastLevelUpParticles_Implementation() const
{
    if (IsValid(LevelUpNiagaraComponent))
    {
        // const FVector CameraLocation = FollowCamera->GetComponentLocation();
        const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
        // const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
        // LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
        LevelUpNiagaraComponent->Activate(true);
    }
}