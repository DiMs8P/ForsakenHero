// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "HealTask.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UHealTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UHealTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
