// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORSAKENHERO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnDeath);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);

public:
	// Sets default values for this component's properties
	UHealthComponent();

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const { return Health;}

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsHealthFull() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float AdditionalHealth);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
	float MaxHealth = 100.0f;

private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* Instigatebly, AActor* DamageCauser);

	void SetHealth(float NewHealth);
};