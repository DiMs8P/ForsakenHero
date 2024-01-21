// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ApplyHealAnimNotify.h"

#include "Character/FHBaseCharacter.h"
#include "Components/InventoryComponent.h"

void UApplyHealAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	if (const AFHBaseCharacter* OwnerCharacter = Cast<AFHBaseCharacter>(MeshComp->GetOwner()))
	{
		UInventoryComponent* InventoryComponent = OwnerCharacter->GetComponentByClass<UInventoryComponent>();
		InventoryComponent->ConsumeItem("HealthPotion");
	}
	
	Super::Notify(MeshComp, Animation, EventReference);
}
