// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShouldDoActionDecorator.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UShouldDoActionDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector CurrentActionKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int RequiredAction;
};
