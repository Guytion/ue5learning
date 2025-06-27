// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAbilitySystemLibrary.h"
#include "Interaction/TeamIdInterface.h"
#include "GameplayEffectTypes.h"
#include "TDMGameplayTags.h"
#include "TDMAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ThreeDMoba/ThreeDMoba.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ThreeDMobaGameMode.h"
#include "Interaction/CombatInterface.h"

void UTDMAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
    /* UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (ClassInfo == nullptr) return;
    
    AActor* AvatarActor = ASC->GetAvatarActor();
    
    FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

    FGameplayEffectContextHandle AttributesContextHandle = ASC->MakeEffectContext();
    AttributesContextHandle.AddSourceObject(AvatarActor);

    const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, AttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

    const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes_Enemy, Level, AttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, AttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get()); */
}

void UTDMAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
    
}

bool UTDMAbilitySystemLibrary::IsFriendly(AActor* ActorA, AActor* ActorB)
{
    if (ActorA->Implements<UTeamIdInterface>() && ActorB->Implements<UTeamIdInterface>())
    {
        return ITeamIdInterface::Execute_GetTeamId(ActorA) == ITeamIdInterface::Execute_GetTeamId(ActorB);
    }
    return false;
}

FGameplayEffectContextHandle UTDMAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    const FTDMGameplayTags& GameplayTags = FTDMGameplayTags::Get();
    const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

    FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContexthandle.AddSourceObject(SourceAvatarActor);
    SetDeathImpulse(EffectContexthandle, DamageEffectParams.DeathImpulse);
    SetIsRadialDamage(EffectContexthandle, DamageEffectParams.bIsRadialDamage);
    SetRadialDamageInnerRadius(EffectContexthandle, DamageEffectParams.RadialDamageInnerRadius);
    SetRadialDamageOuterRadius(EffectContexthandle, DamageEffectParams.RadialDamageOuterRadius);
    SetRadialDamageOrigin(EffectContexthandle, DamageEffectParams.RadialDamageOrigin);

    const FGameplayEffectSpecHandle DamageSpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
        DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage); // BaseDamage随等级变化
    
    DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);
    return EffectContexthandle;
}

FVector UTDMAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const FTDMGameplayEffectContext* TDMContext = static_cast<const FTDMGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return TDMContext->GetDeathImpulse();
    }
    return FVector::ZeroVector;
}

void UTDMAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
    if (FTDMGameplayEffectContext* TDMContext = static_cast<FTDMGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return TDMContext->SetDeathImpulse(InImpulse);
    }
}

void UTDMAbilitySystemLibrary::SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
    ImpulseDirection.Normalize();
    if (Magnitude == 0.f)
    {
        DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
    }
    else
    {
        DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
    }
}

int32 UTDMAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, int32 CharacterLevel)
{
    const AThreeDMobaGameMode* TDMGameMode = Cast<AThreeDMobaGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (TDMGameMode == nullptr)
    {
        UE_LOG(LogThreeDMoba, Warning, TEXT("无法获取游戏模式，无法计算经验值奖励。"));
        return 0;
    }
    const float XPReward = TDMGameMode->XPReward.GetValueAtLevel(CharacterLevel);
    return static_cast<int32>(XPReward);
}

TArray<FRotator> UTDMAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
    TArray<FRotator> Rotators;

    const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
    if (NumRotators > 1)
    {
        const float DeltaSpread = Spread / (NumRotators - 1);
        for (int32 i = 0; i < NumRotators; i++)
        {
            const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
            Rotators.Add(Direction.Rotation());
        }
    }
    else
    {
        Rotators.Add(Forward.Rotation());
    }
    return Rotators;
}

TArray<FVector> UTDMAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
    TArray<FVector> Vectors;

    const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
    if (NumVectors > 1)
    {
        const float DeltaSpread = Spread / (NumVectors - 1);
        for (int32 i = 0; i < NumVectors; i++)
        {
            const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
            Vectors.Add(Direction);
        }
    }
    else
    {
        Vectors.Add(Forward);
    }
    return Vectors;
}

void UTDMAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius, const FVector& SphereOrigin)
{
    FCollisionQueryParams SphereParams;
    SphereParams.AddIgnoredActors(ActorToIgnore);

    TArray<FOverlapResult> Overlaps;
    if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
        for (const FOverlapResult& Overlap : Overlaps)
        {
            if (Overlap.GetActor()->Implements<UCombatInterface>())
            {
                if (!ICombatInterface::Execute_IsDead(Overlap.GetActor()))
                {
                    OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
                }
            }
        }
    }
}

void UTDMAbilitySystemLibrary::SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
    DamageEffectParams.bIsRadialDamage = bIsRadial;
    DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
    DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
    DamageEffectParams.RadialDamageOrigin = Origin;
}

void UTDMAbilitySystemLibrary::SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
    DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

void UTDMAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
    if (FTDMGameplayEffectContext* EffectContext = static_cast<FTDMGameplayEffectContext*>(EffectContextHandle.Get())) // static_cast是C++的类型转换，转换时不能忽视指针属性，和UE5引擎的Cast<>()不同，需要手动处理指针属性。
    {
        EffectContext->SetIsRadialDamage(bInIsRadialDamage);
    }
}

void UTDMAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageInnerRadius)
{
    if (FTDMGameplayEffectContext* EffectContext = static_cast<FTDMGameplayEffectContext*>(EffectContextHandle.Get())) // static_cast是C++的类型转换，转换时不能忽视指针属性，和UE5引擎的Cast<>()不同，需要手动处理指针属性。
    {
        EffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
    }
}

void UTDMAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadialDamageOuterRadius)
{
    if (FTDMGameplayEffectContext* EffectContext = static_cast<FTDMGameplayEffectContext*>(EffectContextHandle.Get())) // static_cast是C++的类型转换，转换时不能忽视指针属性，和UE5引擎的Cast<>()不同，需要手动处理指针属性。
    {
        EffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
    }
}

void UTDMAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin)
{
    if (FTDMGameplayEffectContext* EffectContext = static_cast<FTDMGameplayEffectContext*>(EffectContextHandle.Get())) // static_cast是C++的类型转换，转换时不能忽视指针属性，和UE5引擎的Cast<>()不同，需要手动处理指针属性。
    {
        EffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
    }
}

UAbilityInfo* UTDMAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
    const AThreeDMobaGameMode* GameMode = Cast<AThreeDMobaGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (GameMode == nullptr) return nullptr;

    return GameMode->AbilityInfo;
}