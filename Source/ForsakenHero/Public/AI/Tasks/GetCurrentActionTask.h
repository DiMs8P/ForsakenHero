// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetCurrentActionTask.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UGetCurrentActionTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGetCurrentActionTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector NextActionLocationKey;
};
