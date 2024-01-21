// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/FHMLCharacter.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CreateDataRequestTask.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UCreateDataRequestTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCreateDataRequestTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	float GetDistance(const AFHMLCharacter* MLCharacter, const ACharacter* PlayerCharacter) const;
	float GetPlayerHp(const ACharacter* PlayerCharacter) const;
	float GetSelfHp(const ACharacter* EnemyCharacter) const;
};
