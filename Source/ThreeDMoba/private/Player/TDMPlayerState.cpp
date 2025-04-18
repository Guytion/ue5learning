// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDMPlayerState.h"
#include "Net/UnrealNetwork.h"

ATDMPlayerState::ATDMPlayerState()
{
    NetUpdateFrequency = 100.f;
}

void ATDMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATDMPlayerState, Level); // 设置Level属性为可复制的生命周期属性
    DOREPLIFETIME(ATDMPlayerState, XP);
    DOREPLIFETIME(ATDMPlayerState, AttributePoints);
    DOREPLIFETIME(ATDMPlayerState, SpellPoints);
}

void ATDMPlayerState::OnRep_Level(int32 OldLevel)
{
    OnLevelChangedDelegate.Broadcast(Level);
}

void ATDMPlayerState::OnRep_XP(int32 OldXP)
{
    OnXPChangedDelegate.Broadcast(XP);
}

void ATDMPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDMPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ATDMPlayerState::AddToXP(int32 InXP)
{
    XP += InXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void ATDMPlayerState::SetXP(int32 InXP)
{
    XP = InXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void ATDMPlayerState::AddToLevel(int32 InLevel)
{
    Level += InLevel;
    OnLevelChangedDelegate.Broadcast(Level);
}

void ATDMPlayerState::SetLevel(int32 InLevel)
{
    Level = InLevel;
    OnLevelChangedDelegate.Broadcast(Level);
}

void ATDMPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
    AttributePoints += InAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDMPlayerState::SetAttributePoints(int32 InAttributePoints)
{
    AttributePoints = InAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ATDMPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
    SpellPoints += InSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ATDMPlayerState::SetSpellPoints(int32 InSpellPoints)
{
    SpellPoints = InSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}