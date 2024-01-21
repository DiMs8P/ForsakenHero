// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/LearningDataCollectionSubsystem.h"

#include "FHUtilsFunctionalLibrary.h"
#include "AI/FHBaseAICharacter.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"

void ULearningDataCollectionSubsystem::Start(const AFHBaseAICharacter* EnemyCharacter, const AActor* PlayerCharacter)
{
	UniqueFileIndex = FMath::RandRange(0, 666666);

	UHealthComponent* EnemyHealth = EnemyCharacter->GetComponentByClass<UHealthComponent>();
	UHealthComponent* PlayerHealth = PlayerCharacter->GetComponentByClass<UHealthComponent>();

	const UCombatComponent* CombatComponent = EnemyCharacter->GetComponentByClass<UCombatComponent>();
	
	GetWorld()->GetTimerManager().SetTimer(CollectDataTimerHandle, [this, EnemyCharacter, PlayerCharacter, PlayerHealth, EnemyHealth, CombatComponent]()
	{
		const float Distance = (PlayerCharacter->GetActorLocation() - EnemyCharacter->GetActorLocation()).Length();
		const float PlayerHp = PlayerHealth->GetHealth();
		const float EnemyHp = EnemyHealth->GetHealth();

		const int AttackFlag = GetActionFlag(CombatComponent->GetIsAttacking(), EnemyCharacter->GetIsHealing(), EnemyCharacter->GetIsDodging());
		const FString CollectedData = FString::Printf(TEXT("%.2f %.2f %.2f %d"), Distance, PlayerHp, EnemyHp, AttackFlag);
		
		UFHUtilsFunctionalLibrary::AppendTextToFile(CollectedData, FString::Printf(TEXT("%sPython\\Data\\%s.txt"), *FPaths::ProjectDir(), *FString::FromInt(UniqueFileIndex)));
	}, 1.0f, true);

	EnemyHealth->OnDeath.AddUObject(this, &ULearningDataCollectionSubsystem::Stop);
	PlayerHealth->OnDeath.AddUObject(this, &ULearningDataCollectionSubsystem::Stop);
}

void ULearningDataCollectionSubsystem::Stop()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(CollectDataTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(CollectDataTimerHandle);
	}
}

FORCEINLINE int ULearningDataCollectionSubsystem::GetActionFlag(const bool bIsAttacking, const bool bIsHealing, const bool bIsDodging) const
{
	return bIsAttacking ? 1 : bIsHealing || bIsDodging ? 2 : 0;
}
