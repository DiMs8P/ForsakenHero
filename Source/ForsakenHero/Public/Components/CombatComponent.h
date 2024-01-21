// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORSAKENHERO_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	const ABaseWeapon* GetMainWeapon() const;
	bool GetIsAttacking() const;
	
	void Attack();
	void ContinueAttack();
	void ResetAttack();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void Attack_Internal(ACharacter* Owner, int AttackIndex, bool IsIndexRandom = false);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABaseWeapon> MainWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ABaseWeapon* MainWeapon;

private:
	int AttackCount = 0;
	bool bIsAttackSaved = false;
	bool bIsAttacking = false;
};
