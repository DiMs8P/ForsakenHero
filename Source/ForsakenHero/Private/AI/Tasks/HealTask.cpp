// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/HealTask.h"
#include "AIController.h"
#include "Character/FHBaseCharacter.h"

UHealTask::UHealTask()
{
	NodeName = "Heal";
}

EBTNodeResult::Type UHealTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	const auto BaseCharacter = Cast<AFHBaseCharacter>(Controller->GetPawn());
	if (!BaseCharacter)
	{
		return EBTNodeResult::Failed;
	}

	BaseCharacter->Heal();
	return EBTNodeResult::Succeeded;
}
