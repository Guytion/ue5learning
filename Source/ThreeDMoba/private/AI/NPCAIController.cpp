// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/Character.h"
#include "Interaction/TeamIdInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

ANPCAIController::ANPCAIController()
{
    AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI感知组件"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI视力配置"));
    SightConfig->SightRadius = 1500.f;
    SightConfig->LoseSightRadius = 1700.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    AIPerComp->ConfigureSense(*SightConfig);

    AIPerComp->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::OnTargetPerceptionUpdated);

}

void ANPCAIController::BeginPlay()
{
    Super::BeginPlay();

    

    if (BTAsset != nullptr)
    {
        bool RunResult = RunBehaviorTree(BTAsset);
    }
    // UE_LOG(LogTemp, Warning, TEXT("被控制的角色是：%s"), *GetCharacter()->GetName())
}

void ANPCAIController::OnTargetPerceptionUpdated(AActor* Actor, const struct FAIStimulus Stimulus)
{
    ACharacter* TargetChar = Cast<ACharacter>(Actor);
    if (TargetChar == nullptr)
    {
        return;
    }
    ACharacter* MyChar = GetCharacter();
    if (MyChar == nullptr)
    {
        return;
    }
    if (TargetChar->Implements<UTeamIdInterface>() && MyChar->Implements<UTeamIdInterface>())
    {
        if (ITeamIdInterface::Execute_GetTeamId(TargetChar) != ITeamIdInterface::Execute_GetTeamId(MyChar))
        {
            // 设置黑板上的目标
            UBlackboardComponent* BlackBoard = GetBlackboardComponent();
            BlackBoard->SetValueAsObject("TargetActor", TargetChar); // 设置目标角色
        }
        
    }
    
}