// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/ShouldHealDecorator.h"
#include "AIController.h"
#include "AI/FHBaseAICharacter.h"
#include "Components/HealthComponent.h"

bool UShouldHealDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return false;
	}

	const auto BaseAICharacter = Cast<AFHBaseAICharacter>(Controller->GetPawn());
	if (!BaseAICharacter)
	{
		return false;
	}

	const UHealthComponent* HealthComponent = BaseAICharacter->GetComponentByClass<UHealthComponent>();
	if (!HealthComponent)
	{
		return false;
	}
	
	return HealthComponent->GetHealth() < RequiredHealth && BaseAICharacter->CanPerformHeal();
}
