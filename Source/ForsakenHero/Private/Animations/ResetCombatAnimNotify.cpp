// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ResetCombatAnimNotify.h"

#include "Character/FHBaseCharacter.h"

void UResetCombatAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	if (AFHBaseCharacter* OwnerCharacter = Cast<AFHBaseCharacter>(MeshComp->GetOwner()))
	{
		OwnerCharacter->ResetCombat();
	}

	Super::Notify(MeshComp, Animation, EventReference);
}
