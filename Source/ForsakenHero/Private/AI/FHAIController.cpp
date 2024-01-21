// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\AI\FHAIController.h"

ABaseAIController::ABaseAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseAIController::BeginPlay()
{
	RunBehaviorTree(EnemyBehaviorTree);

	Super::BeginPlay();
}
