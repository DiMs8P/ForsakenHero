// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/BaseWeapon.h"

#include "Character/FHBaseCharacter.h"
#include "Components/CollisionComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UCollisionComponent>(TEXT("CollisionComponent"));
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnHitDelegate.AddUObject(this, &ABaseWeapon::OnHit);
}

void ABaseWeapon::OnHit(const FHitResult& HitResult)
{
	if (AFHBaseCharacter* HitActor = Cast<AFHBaseCharacter>(HitResult.GetActor()))
	{
		if(HitActor->CanReceiveDamage())
		{
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, GetActorForwardVector(), HitResult, GetInstigatorController(), this, {});
		}
	}
}

void ABaseWeapon::OnEquipped()
{
	Super::OnEquipped();

	CollisionComponent->SetCollisionMesh(GetItemMesh());
	CollisionComponent->AddActorToIgnore(GetOwner());
}

const TArray<UAnimMontage*>& ABaseWeapon::GetAttackMontages() const
{
	return AttackMontages;
}

UCollisionComponent* ABaseWeapon::GetCollisionComponent() const
{
	return CollisionComponent;
}


