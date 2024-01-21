﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseConsumable.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FORSAKENHERO_API UBaseConsumable : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnConsume(UObject* WorldContextObject);
};
