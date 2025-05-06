// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TDMAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "TDMGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"

UTDMAttributeSet::UTDMAttributeSet()
{
    const FTDMGameplayTags& GameplayTags = FTDMGameplayTags::Get();

    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
    TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
}

void UTDMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UTDMAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDMAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDMAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDMAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDMAttributeSet, Qi, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTDMAttributeSet, MaxQi, COND_None, REPNOTIFY_Always);
}

void UTDMAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDMAttributeSet, Health, OldHealth);
}

void UTDMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDMAttributeSet, MaxHealth, OldMaxHealth);
}

void UTDMAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDMAttributeSet, Mana, OldMana);
}

void UTDMAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDMAttributeSet, MaxMana, OldMaxMana);
}

void UTDMAttributeSet::OnRep_Qi(const FGameplayAttributeData& OldQi) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDMAttributeSet, Qi, OldQi);
}

void UTDMAttributeSet::OnRep_MaxQi(const FGameplayAttributeData& OldMaxQi) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTDMAttributeSet, MaxQi, OldMaxQi);
}

void UTDMAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    if (Attribute == GetManaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
    }
}

void UTDMAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
    // 将Source和Target的控制器、角色、技能系统信息设置到Props中。
    // Source是效果的施加者，Target是效果的目标。如果Source和Target是同一个Actor，那么这个效果就是自增的效果。（此属性集的拥有者）

    Props.EffectContextHandle = Data.EffectSpec.GetContext();
    Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

    if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && IsValid(Props.SourceASC->GetAvatarActor()))
    {
        // AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
        Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor(); 
        // UE_LOG(LogTemp, Warning, TEXT("SourceAvatarActor: %s, DActor: %s"), *GetNameSafe(SourceAvatarActor), *GetNameSafe(DActor));

        Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
        if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
        {
            if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
            {
                Props.SourceController = Pawn->GetController(); // 获取控制这个Pawn的Controller，通常是AI或玩家控制器。如果Pawn是自增效果的目标，则不需要获取Controller，因为Controller可能不存在或未初始化。
            }
        }
        if (Props.SourceController)
        {
            Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
        }
    }

    if (IsValid(Data.Target.GetAvatarActor()) && Data.Target.AbilityActorInfo.IsValid())
    {
        Props.TargetAvatarActor = Data.Target.GetAvatarActor();

        Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
        Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
    }
}

void UTDMAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FEffectProperties Props;
    SetEffectProperties(Data, Props);

    /* if (Props.TargetCharacter->Implements<UCombatInterface>())
    {
        if (ICombatInterface::Execute_IsDead(Props.TargetCharacter))
        {
            return;
        }
    } */
    
    bool bFatal = false; // 是否致命

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        bFatal = GetHealth() <= 0.f;
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        bFatal = HandleIncomingDamage(Props) || bFatal;
    }
    if (bFatal)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Dead")));
    }
    
}

void UTDMAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    if (Attribute == GetMaxHealthAttribute())
    {
        SetHealth(GetMaxHealth());
    }
    if (Attribute == GetMaxManaAttribute())
    {
        SetMana(GetMaxMana());
    }
    if (Attribute == GetQiAttribute())
    {
        //TODO: 如果气大于最大值，则层数+1，气减最大值
    }
}

bool UTDMAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
    const float LocalIncomingDamage = GetIncomingDamage();
    SetIncomingDamage(0.f);
    if (LocalIncomingDamage > 0.f)
    {
        const float NewHealth = GetHealth() - LocalIncomingDamage; // 伤害结算
        // UE_LOG(LogTemp, Warning, TEXT("生命值：%f, 最大生命值：%f，受到%f伤害"), GetHealth(), GetMaxHealth(), LocalIncomingDamage);
        SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

        const bool bFatal = NewHealth <= 0.f;
        if (!bFatal)
        {
            // 受击反馈
            if (Props.TargetCharacter->Implements<UCombatInterface>())
            {
                FGameplayTagContainer TagContainer;
                TagContainer.AddTag(FTDMGameplayTags::Get().Effects_HitReact);
                Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
            }
        }
        return bFatal;
    }
    return false;
}