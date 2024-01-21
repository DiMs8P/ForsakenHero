// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FHBaseAICharacter.h"
#include "FHAICharacter.generated.h"

UCLASS()
class FORSAKENHERO_API AFHAICharacter : public AFHBaseAICharacter
{
	GENERATED_BODY()

public:
	AFHAICharacter();

protected:
	virtual void BeginPlay() override;

	virtual FText GetEnemyLabel() const override;
};
