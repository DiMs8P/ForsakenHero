// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Katana.generated.h"

UCLASS()
class FORSAKENHERO_API AKatana : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AKatana();

protected:
	virtual void BeginPlay() override;
};
