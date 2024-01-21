// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FHBaseAICharacter.h"

#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/EnemyWidget.h"

AFHBaseAICharacter::AFHBaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidgetComponent->SetupAttachment(GetRootComponent());
}

void AFHBaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	const UEnemyWidget* EnemyWidget = Cast<UEnemyWidget>(EnemyWidgetComponent->GetWidget());
	EnemyWidget->SetEnemyLabel(GetEnemyLabel());
}

void AFHBaseAICharacter::Tick(float DeltaSeconds)
{
	const UEnemyWidget* EnemyWidget = Cast<UEnemyWidget>(EnemyWidgetComponent->GetWidget());
	EnemyWidget->SetHealthPercent(HealthComponent->GetHealthPercent());

	EnemyWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(),
			UGameplayStatics::GetPlayerCameraManager(this, 0)->GetTransformComponent()->GetComponentLocation()));
	
	Super::Tick(DeltaSeconds);
}

FRotator AFHBaseAICharacter::GetDesiredRotation() const
{
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	return UKismetMathLibrary::MakeRotFromX(GetActorLocation() - PlayerCharacter->GetActorLocation());
}

FORCEINLINE FText AFHBaseAICharacter::GetEnemyLabel() const
{
	return FText::FromString("BaseEnemy");
}
