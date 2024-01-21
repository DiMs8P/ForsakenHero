// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "GameFramework/Character.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "Character/FHBaseCharacter.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
		ActorSpawnParameters.Instigator = Owner;
		ActorSpawnParameters.Owner = Owner;
		
		MainWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MainWeaponClass, Owner->GetTransform(), ActorSpawnParameters);

		MainWeapon->OnEquipped();
	}
	
	Super::BeginPlay();
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	MainWeapon->Destroy();
	
	Super::EndPlay(EndPlayReason);
}

void UCombatComponent::Attack()
{
	if (GetIsAttacking())
	{
		bIsAttackSaved = true;	
	}
	else
	{
		AFHBaseCharacter* Owner = Cast<AFHBaseCharacter>(GetOwner());
		if (Owner->CanPerformAction())
		{
			Attack_Internal(Owner, AttackCount);
		}
	}
}

void UCombatComponent::Attack_Internal(ACharacter* Owner, int AttackIndex, bool IsIndexRandom)
{
	const TArray<UAnimMontage*>& AttackAnimMontages = GetMainWeapon()->GetAttackMontages();

	UAnimMontage* SelectedAttackAnimMontage = IsIndexRandom ? AttackAnimMontages[FMath::RandRange(0, AttackAnimMontages.Num() - 1)] : AttackAnimMontages[AttackIndex];
	Owner->PlayAnimMontage(SelectedAttackAnimMontage);

	bIsAttacking = true;
	AttackCount = AttackCount + 1 >= AttackAnimMontages.Num() ? 0 : AttackCount + 1;
}

void UCombatComponent::ContinueAttack()
{
	bIsAttacking = false;

	if (bIsAttackSaved)
	{
		bIsAttackSaved = false;
		Attack();
	}
}

void UCombatComponent::ResetAttack()
{
	AttackCount = 0;
	
	bIsAttacking = false;
	bIsAttackSaved = false;
}

FORCEINLINE const ABaseWeapon* UCombatComponent::GetMainWeapon() const
{
	return MainWeapon;
}

FORCEINLINE bool UCombatComponent::GetIsAttacking() const
{
	return bIsAttacking;
}
