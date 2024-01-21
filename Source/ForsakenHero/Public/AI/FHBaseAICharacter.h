// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FHBaseCharacter.h"
#include "FHBaseAICharacter.generated.h"

UCLASS()
class FORSAKENHERO_API AFHBaseAICharacter : public AFHBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFHBaseAICharacter();

	virtual FRotator GetDesiredRotation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FText GetEnemyLabel() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* EnemyWidgetComponent;
};