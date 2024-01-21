// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RollBackTask.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API URollBackTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URollBackTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
