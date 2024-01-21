// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\AI\Tasks\GetCurrentActionTask.h"

#include "AIController.h"
#include "AI/FHMLCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UGetCurrentActionTask::UGetCurrentActionTask()
{
	NodeName = "Get Current Model Action";
}

EBTNodeResult::Type UGetCurrentActionTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	const auto MLCharacter = Cast<AFHMLCharacter>(Controller->GetPawn());
	if (!MLCharacter)
	{
		return EBTNodeResult::Failed;
	}

	MLCharacter->SetIsModelActualData(false);
	Blackboard->SetValueAsInt(NextActionLocationKey.SelectedKeyName, MLCharacter->GetAction());
	
	return EBTNodeResult::Succeeded;
}
