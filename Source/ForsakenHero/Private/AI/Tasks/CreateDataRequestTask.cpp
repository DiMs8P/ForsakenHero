// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/CreateDataRequestTask.h"

#include "AIController.h"
#include "AI/FHMLCharacter.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

UCreateDataRequestTask::UCreateDataRequestTask()
{
	NodeName = "Create Data Request";
}

EBTNodeResult::Type UCreateDataRequestTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();

	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	const auto MLCharacter = Cast<AFHMLCharacter>(Controller->GetPawn());
	if (!MLCharacter)
	{
		return EBTNodeResult::Failed;
	}

	if(!MLCharacter->GetIsModelActualData() && !MLCharacter->GetIsModelRunning())
	{
		const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if(!PlayerCharacter)
		{
			return EBTNodeResult::Failed;
		}

		MLCharacter->UpdateInputData(
			GetDistance(MLCharacter, PlayerCharacter),
			GetPlayerHp(PlayerCharacter),
			GetSelfHp(MLCharacter));

		MLCharacter->RunModel();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

float UCreateDataRequestTask::GetDistance(const AFHMLCharacter* MLCharacter, const ACharacter* PlayerCharacter) const
{
	return (MLCharacter->GetActorLocation() - PlayerCharacter->GetActorLocation()).Length();
}

float UCreateDataRequestTask::GetPlayerHp(const ACharacter* PlayerCharacter) const
{
	const UHealthComponent* PlayerHealthComponent = PlayerCharacter->GetComponentByClass<UHealthComponent>();

	return PlayerHealthComponent->GetHealth();
}

float UCreateDataRequestTask::GetSelfHp(const ACharacter* EnemyCharacter) const
{
	const UHealthComponent* EnemyHealthComponent = EnemyCharacter->GetComponentByClass<UHealthComponent>();

	return EnemyHealthComponent->GetHealth();
}
