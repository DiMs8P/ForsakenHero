// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseConsumable.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FORSAKENHERO_API UHealthPotion : public UBaseConsumable
{
	GENERATED_BODY()

public:
	virtual void OnConsume(UObject* WorldContextObject) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditionalHealth = 50;
};
