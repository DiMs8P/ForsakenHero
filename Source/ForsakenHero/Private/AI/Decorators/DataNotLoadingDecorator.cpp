// Fill out your copyright notice in the Description page of Project Settings.


#include "AI\Decorators\DataNotLoadingDecorator.h"

#include "AIController.h"
#include "AI/FHMLCharacter.h"

bool UDataNotLoadingDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();

	if (!Controller)
	{
		return false;
	}

	const auto MLCharacter = Cast<AFHMLCharacter>(Controller->GetPawn());
	if (!MLCharacter)
	{
		return false;
	}
	
	return MLCharacter->GetIsModelActualData() && !MLCharacter->GetIsModelRunning();
}
