// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercent(const float Percent) const;
	void SetEnemyLabel(const FText& NewLabel) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* EnemyLabel;
};
