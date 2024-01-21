// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CollisionTraceAnimNotifyState.h"

#include "Character/FHBaseCharacter.h"
#include "Components/CollisionComponent.h"
#include "Components/CombatComponent.h"
#include "Actors/Weapons/BaseWeapon.h"

void UCollisionTraceAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UCollisionComponent* CollisionComponent = GetCollisionComponent(MeshComp->GetOwner());
	CollisionComponent->EnableCollision();
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UCollisionTraceAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UCollisionComponent* CollisionComponent = GetCollisionComponent(MeshComp->GetOwner());
	CollisionComponent->DisableCollision();
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

UCollisionComponent* UCollisionTraceAnimNotifyState::GetCollisionComponent(const AActor* Owner) const
{
	const AFHBaseCharacter* OwnerCharacter = Cast<AFHBaseCharacter>(Owner);
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Can't cast weapon owner to BaseCharacter"))
	}

	const UCombatComponent* CombatComponent = OwnerCharacter->GetComponentByClass<UCombatComponent>();
	if (!CombatComponent)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Can't get combat component"))
	}

	const ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
	if (!MainWeapon)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Main weapon is not attached"))
	}

	UCollisionComponent* CollisionComponent = MainWeapon->GetCollisionComponent();
	if (!CollisionComponent)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Collision component is not attached"))
	}

	return CollisionComponent;
}
