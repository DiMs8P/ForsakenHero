// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LearningDataCollectionSubsystem.generated.h"

UCLASS()
class FORSAKENHERO_API ULearningDataCollectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Start(const class AFHBaseAICharacter* EnemyCharacter, const AActor* PlayerCharacter);
	void Stop();

private:
	int GetActionFlag(const bool bIsAttacking, const bool bIsHealing, const bool bIsDodging) const;

private:
	int UniqueFileIndex;
	FTimerHandle CollectDataTimerHandle;
};
