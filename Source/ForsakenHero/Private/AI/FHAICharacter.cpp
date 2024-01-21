// Fill out your copyright notice in the Description page of Project Settings.


#include "AI\FHAICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/LearningDataCollectionSubsystem.h"

AFHAICharacter::AFHAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFHAICharacter::BeginPlay()
{
	Super::BeginPlay();

	/*ULearningDataCollectionSubsystem* LearningDataCollectionSubsystem = GetGameInstance()->GetSubsystem<ULearningDataCollectionSubsystem>();
	LearningDataCollectionSubsystem->Start(this, UGameplayStatics::GetPlayerCharacter(this, 0));*/
}

FORCEINLINE FText AFHAICharacter::GetEnemyLabel() const
{
	return FText::FromString("AI");
}
