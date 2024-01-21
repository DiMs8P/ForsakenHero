// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\AI\Tasks\GetTargetTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UGetTargetTask::UGetTargetTask()
{
	NodeName = "Get Target";
}

EBTNodeResult::Type UGetTargetTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsObject(PlayerKey.SelectedKeyName, UGameplayStatics::GetPlayerCharacter(this, 0));
	return EBTNodeResult::Succeeded;
}
