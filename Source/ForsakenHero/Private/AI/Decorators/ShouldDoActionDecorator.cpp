// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/ShouldDoActionDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UShouldDoActionDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Blackboard)
	{
		return false;
	}
	
	return RequiredAction == Blackboard->GetValueAsInt(CurrentActionKey.SelectedKeyName);
}
