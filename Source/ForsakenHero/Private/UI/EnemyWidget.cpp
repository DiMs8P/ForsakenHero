// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyWidget::SetHealthPercent(const float Percent) const
{
	HealthProgressBar->SetPercent(Percent);
}

void UEnemyWidget::SetEnemyLabel(const FText& NewLabel) const
{
	EnemyLabel->SetText(NewLabel);
}
