// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseEquippable.h"
#include "BaseWeapon.generated.h"

UCLASS()
class FORSAKENHERO_API ABaseWeapon : public ABaseEquippable
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	virtual void OnEquipped() override;

	const TArray<class UAnimMontage*>& GetAttackMontages() const;
	class UCollisionComponent* GetCollisionComponent() const;

protected:
	virtual void BeginPlay() override;

private:
	void OnHit(const FHitResult& HitResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UCollisionComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Damage;
};
