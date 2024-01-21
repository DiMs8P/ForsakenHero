// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseEquippable.h"

#include "GameFramework/Character.h"

ABaseEquippable::ABaseEquippable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

void ABaseEquippable::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEquippable::OnEquipped()
{
	SetIsEquipped(true);
	AttachActor(AttachSocketName);
}

void ABaseEquippable::AttachActor(const FName& SocketName)
{
	const ACharacter* BaseCharacter = Cast<ACharacter>(GetOwner());

	if (USkeletalMeshComponent* OwnerSkeletalMesh = BaseCharacter->GetMesh())
	{
		const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(OwnerSkeletalMesh, AttachmentTransformRules, SocketName);
	}
}
void ABaseEquippable::OnUnequipped()
{
	if (bIsEquipped)
	{
		bIsEquipped = false;
	}
}

FORCEINLINE bool ABaseEquippable::GetIsEquipped()
{
	return bIsEquipped;
}

FORCEINLINE UPrimitiveComponent* ABaseEquippable::GetItemMesh()
{
	if (StaticMeshComponent->GetStaticMesh()->IsValidLowLevel())
	{
		return StaticMeshComponent;
	}
	
	return SkeletalMeshComponent;
}

FORCEINLINE void ABaseEquippable::SetIsEquipped(bool IsEquipped)
{
	bIsEquipped = IsEquipped;
}
