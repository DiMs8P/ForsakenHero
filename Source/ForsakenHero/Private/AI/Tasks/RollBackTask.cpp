// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/RollBackTask.h"
#include "AIController.h"
#include "Character/FHBaseCharacter.h"

URollBackTask::URollBackTask()
{
	NodeName = "Dodge";
}

EBTNodeResult::Type URollBackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	const auto BaseAICharacter = Cast<AFHBaseCharacter>(Controller->GetPawn());
	if (!BaseAICharacter)
	{
		return EBTNodeResult::Failed;
	}
	
	BaseAICharacter->Dodge();
	return EBTNodeResult::Succeeded;
}
