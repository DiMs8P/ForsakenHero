// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/AttackTask.h"
#include "AIController.h"
#include "Components/CombatComponent.h"

UAttackTask::UAttackTask()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	const APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	UCombatComponent* CombatComponent = Pawn->GetComponentByClass<UCombatComponent>();
	if (!CombatComponent)
	{
		return EBTNodeResult::Failed;
	}
	
	CombatComponent->Attack();
	return EBTNodeResult::Succeeded;
}
