// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FHBaseCharacter.generated.h"

class UCombatComponent;

UCLASS()
class FORSAKENHERO_API AFHBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFHBaseCharacter();

	void ResetCombat();
	
	bool CanReceiveDamage() const;

	void Dodge();
	void Heal();

	bool CanPerformAction() const;
	bool CanPerformHeal() const;

	bool GetIsHealing() const {return bIsHealing;}
	bool GetIsDodging() const {return bIsDodging;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnDeath();

private:
	void Dodge_Internal(int DodgeIndex, bool IsIndexRandom = false);
	void Heal_Internal(int HealIndex, bool IsIndexRandom = false);
	
	virtual FRotator GetDesiredRotation() const;

	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* Instigatebly, AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> DeathMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> DodgeMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> HealingMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> HitMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsDodging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool IsDisabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsHealing;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float LifeSpanOnDeath = 5.0f;
};
